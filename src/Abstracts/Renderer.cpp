#include "IShaderProgram.h"
#include "IShaderManager.h"

#include "Renderer.h"

namespace zge
{

Renderer::~Renderer() NOEXCEPT {}

const bool Renderer::initialise()
{
  return IShaderManager::getInstance()->initialise();
}

void Renderer::addInstance(unsigned shaderId, IGameObject* gameObject)
{
  IShaderManager::getInstance()->addInstance(shaderId, gameObject);
}

void Renderer::render() { IShaderManager::getInstance()->executeShaders(); }

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

const unsigned Renderer::getRasterState() const
{
  return this->rasterState;
}

} // namespace zge
