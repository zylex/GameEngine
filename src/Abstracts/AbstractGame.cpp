#include <iostream>

#include "Preprocessors.h"

#include "IGameState.h"
#include "IInputManager.h"
#include "IRenderer.h"

#include "AbstractGame.h"

namespace zge
{

AbstractGame::AbstractGame()
    : running(true), nextState(0), currentState(nullptr), windowTitle("")
{
}

AbstractGame::~AbstractGame() NOEXCEPT
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

const bool AbstractGame::initialise()
{
  if (not IInputManager::getInstance()->clear())
  {
    return false;
  }

  if (not IRenderer::getInstance()->initialise())
  {
    return false;
  }

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

const int AbstractGame::run()
{
  // if (not this->states.empty() and this->initialise())
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

void AbstractGame::frame()
{
  // NOTE: override this method if input handling is needed
  this->currentState->update();

  IRenderer::getInstance()->render();

  this->changeState();
}

void AbstractGame::setWindowTitle(std::string windowTitle)
{

  this->windowTitle = windowTitle;
}

const std::string AbstractGame::getWindowTitle() const
{
  return this->windowTitle;
}

void AbstractGame::addGameState(const int key, IGameState* state)
{
  this->states[key] = state;
}

void AbstractGame::setNextState(const int nextState)
{
  this->nextState = nextState;
}

IGameState* AbstractGame::getCurrentGameState() const
{
  return this->currentState;
}

void AbstractGame::changeState()
{
  if (this->nextState IS_NOT 0 and
      this->states.find(this->nextState) IS_NOT this->states.end())
  {
    this->currentState = this->states[this->nextState];
    this->nextState = 0;
  }
}

const bool AbstractGame::isRunning() const { return this->running; }

void AbstractGame::quit() { this->running = false; }

const std::unordered_map<int, IGameState*> AbstractGame::getStates()
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