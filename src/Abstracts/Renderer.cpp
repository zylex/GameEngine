#include <AntTweakBar.h>

#include "IGame.h"
#include "IGameState.h"
#include "IShaderProgram.h"
#include "IShaderManager.h"

#include "Renderer.h"

#include "Stats.h"

namespace zge
{

Renderer::~Renderer() NOEXCEPT
{
#ifdef USE_ANT
  if (this->twNotTerminated)
  {
    TwTerminate();
    this->twNotTerminated = false;
  }
#endif
}

const bool Renderer::initialise()
{
#ifdef USE_ANT
  this->showAntTweakBar = false;
  this->twNotTerminated = true;
#endif
  return IShaderManager::getInstance()->initialise();
}

void Renderer::addInstance(unsigned shaderId, IGameObject* gameObject)
{
  IShaderManager::getInstance()->addInstance(shaderId, gameObject);
}

void Renderer::render()
{
  IShaderManager::getInstance()->executeShaders();
  IGame::getInstance()->getCurrentGameState()->cleanAll();
#ifdef USE_ANT
  if (this->showAntTweakBar)
  {
    TwDraw();
  }
#endif
  this->swapBuffers();
}

const unsigned Renderer::getDepthState() const { return this->depthState; }

void Renderer::setDepthState(const unsigned depthState)
{
  this->depthState = depthState;
}

void Renderer::setShaderProgram(const unsigned programId)
{
  this->currentShaderProgramId = programId;
}

const unsigned Renderer::getShaderProgramId() const
{
  return this->currentShaderProgramId;
}

void Renderer::setRasterState(const unsigned rasterState)
{
  this->rasterState = rasterState;
}

const unsigned Renderer::getRasterState() const { return this->rasterState; }

void Renderer::setAlphaState(const unsigned alphaState)
{
  this->alphaState = alphaState;
}

const unsigned Renderer::getAlphaState() const { return this->alphaState; }

#ifdef USE_ANT
void Renderer::toggleAntTweakBar()
{
  this->showAntTweakBar = not this->showAntTweakBar;
}

#endif

} // namespace zge
