#include <iostream>

#ifdef USE_ANT
#include "Stats.h"
#endif

#include "Preprocessors.h"

#include "Game.h"

#include "IGameState.h"
#include "IInputManager.h"
#include "IRenderer.h"
#include "IResourceManager.h"
#include "IGlobalTimer.h"

#include "ThreadManager.h"

namespace zge
{

Game::Game()
    : running(true), nextState(0), currentState(nullptr), windowTitle("")
{
  // TODO: constructor: set thread affinity to 0
}

Game::~Game() NOEXCEPT { this->shutdown(); }

void Game::shutdown()
{
  if (this->states.size() > 0)
  {
    std::unordered_map<int, IGameState*>::iterator stateIterator;
    for (stateIterator = this->states.begin();
         stateIterator != this->states.end(); stateIterator++)
    {
      SAFE_DELETE(stateIterator->second);
    }
    this->states.clear();
  }

  this->currentState = nullptr;
}

const bool Game::initialise()
{
  int coreNumber = 1;
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) ||               \
                         (defined(__APPLE__) && defined(__MACH__)))
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(coreNumber, &cpuset);
  int result =
      pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
  if (result)
  {
    std::cerr << "ERROR: error set visualization thread affinity, error code: "
              << result << std::endl;
  }
#elif defined(_WIN32) || defined(_WIN64)
  DWORD result = SetAffinityMask(GetCurrentThread(), (1 << this->CORE_NUMBER));
  if (FAILED(result))
  {
    std::cerr << "ERROR: error set visualization thread affinity, error code: "
              << result << std::endl;
  }
#endif
  // start the clock
  if (not IGlobalTimer::getInstance())
  {
    return false;
  }

  if (not IInputManager::getInstance()->clear())
  {
    return false;
  }

  if (not IRenderer::getInstance()->initialise())
  {
    return false;
  }

  if (not IResourceManager::getInstance()->initialise())
  {
    return false;
  }

  IRenderer::getInstance()->enableDepth();
  IRenderer::getInstance()->enableAntiClockwiseCulling();

  std::unordered_map<int, IGameState*>::iterator stateIterator;
  for (stateIterator = this->states.begin();
       stateIterator != this->states.end(); stateIterator++)
  {
    if (not stateIterator->second->initialise())
    {
      return false;
    }
  }

  return true;
}

const int Game::run()
{
  if (not this->states.empty())
  {
    if (this->initialise())
    {
      this->changeState();
      // TODO: Game::run: Start the other threads
      ThreadManager::getInstance().startAll();

      while (this->isRunning())
      {
        this->frame();
      }
      // TODO: Game::run: join other threads
      ThreadManager::getInstance().joinAll();
      return EXIT_SUCCESS;
    }
    else
    {
      std::cerr << "Failed to initialise the game." << std::endl;
      return EXIT_FAILURE;
    }
  }
  else
  {
    std::cerr << "No states/scenes added to game." << std::endl;
    return EXIT_FAILURE;
  }
}

void Game::frame()
{
  // NOTE: override this method if input handling is needed
  this->currentState->update();

  IRenderer::getInstance()->render();

  // this->currentState->cleanAll();

  this->changeState();

#ifdef USE_ANT
  Stats::getInstance()->mainThreadTick();
#endif
}

void Game::setWindowTitle(std::string windowTitle)
{
  this->windowTitle = windowTitle;
}

const std::string& Game::getWindowTitle() const { return this->windowTitle; }

void Game::addGameState(const int key, IGameState* state)
{
  this->states[key] = state;
}

void Game::setNextState(const int nextState) { this->nextState = nextState; }

IGameState* Game::getCurrentGameState() const { return this->currentState; }

void Game::changeState()
{
  if (this->nextState IS_NOT 0 and
      this->states.find(this->nextState) IS_NOT this->states.end())
  {
    this->currentState = this->states[this->nextState];
    this->nextState = 0;
  }
}

const bool Game::isRunning() const { return this->running; }

void Game::quit() { this->running = false; }

const std::unordered_map<int, IGameState*> Game::getStates()
{
  return this->states;
}

const int SCREEN_WIDTH(const int width)
{
  static int screen_width = width;
  return screen_width;
}

const int SCREEN_HEIGHT(const int height)
{
  static int screen_height = height;
  return screen_height;
}

} // namespace zge
