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

} // namespace zge