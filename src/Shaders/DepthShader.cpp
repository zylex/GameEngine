#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "DepthShader.h"

#include "IGame.h"
#include "IGameState.h"
#include "IRenderer.h"
#include "IResourceManager.h"

#include "DepthState.h"
#include "ShaderType.h"
#include "TextureType.h"
#include "Camera.h"

#include "InterpolatedVS.h"

namespace zge
{

DepthShader::DepthShader()
{
  // constructor
}

DepthShader::~DepthShader() NOEXCEPT
{
  // destructor
}

DepthShader::DepthShader(const DepthShader& other)
{
  // copy constructor
}

DepthShader& DepthShader::operator=(const DepthShader& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

DepthShader::DepthShader(DepthShader&& other) NOEXCEPT
{
  // move constructor (C++11)
}

DepthShader& DepthShader::operator=(DepthShader&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new DepthShader(other));
}

const bool DepthShader::initialise()
{
  this->clear();

  IResourceManager* resourceManager = IResourceManager::getInstance();
  unsigned vertexShaderId = resourceManager->compileShaderCode(
      InterpolatedVS, InterpolatedVS_size, VERTEX_SHADER);
  if (not vertexShaderId)
  {
    return false;
  }

  unsigned programId =
      resourceManager->createShaderProgram(0, vertexShaderId, 0, 0, 0, 0);
  if (not programId)
  {
    return false;
  }

  this->depthTexture = resourceManager->createRawTexture2D(
      SCREEN_WIDTH(), SCREEN_HEIGHT(), ZGE_DEPTH);
  this->setOutputId(
      resourceManager->createOutputBuffer(this->depthTexture, {}));

  this->setProgramId(programId);

  this->setDepthState(ZGE_DEPTH_ON);

  return true;
}

void DepthShader::prepare()
{
  MeshShader::prepare();
  // set constants
  IRenderer::getInstance()->setConstant(
      0, new glm::mat4(IGame::getInstance()
                           ->getCurrentGameState()
                           ->getCurrentCamera()
                           ->getViewMatrix()),
      sizeof(glm::mat4), VERTEX_SHADER);
}

const unsigned DepthShader::getDepthTexture() { return this->depthTexture; }

} // namespace zge
