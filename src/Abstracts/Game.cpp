#include <iostream>

#include "Preprocessors.h"

#include "IGameState.h"
#include "IInputManager.h"
#include "IRenderer.h"
#include "IResourceManager.h"

#include "Game.h"

namespace zge
{

Game::Game()
    : running(true), nextState(0), currentState(nullptr), windowTitle("")
{
}

Game::~Game() NOEXCEPT
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
  IRenderer::getInstance()->enableClockwiseCulling();

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
      while (this->isRunning())
      {
        this->frame();
      }
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

  this->changeState();
}

void Game::setWindowTitle(std::string windowTitle)
{
  this->windowTitle = windowTitle;
}

const std::string Game::getWindowTitle() const { return this->windowTitle; }

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
