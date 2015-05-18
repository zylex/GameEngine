#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include <glm/gtc/type_ptr.hpp>

#include "ShaderType.h"
#include "SamplerType.h"

#include "IGame.h"
#include "IRenderer.h"
#include "IResourceManager.h"
#include "IGameState.h"
#include "IGameObject.h"
#include "IShaderManager.h"

#include "DepthState.h"
#include "TextureType.h"
#include "Shader.h"
#include "Camera.h"

#include "PsychedaelicShader.h"

#include "InterpolatedVS.h"
#include "InterpolatedDepthPS.h"

#include "DepthShader.h"

namespace zge
{

PsychedaelicShader::PsychedaelicShader(unsigned textureId)
    : showDepth(false), depthTextureId(textureId)
{

  // constructor
}

PsychedaelicShader::~PsychedaelicShader() NOEXCEPT
{
  // destructor
  this->clear();
}

PsychedaelicShader::PsychedaelicShader(const PsychedaelicShader& other)
{

  // copy constructor
}

PsychedaelicShader& PsychedaelicShader::operator=(const PsychedaelicShader& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

PsychedaelicShader::PsychedaelicShader(PsychedaelicShader&& other) NOEXCEPT
{

  // move constructor (C++11)
}

PsychedaelicShader& PsychedaelicShader::operator=(
    PsychedaelicShader&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new PsychedaelicShader(other));
}

const bool PsychedaelicShader::initialise()
{
  this->clear();

  IResourceManager* resourceManager = IResourceManager::getInstance();
  unsigned vertexShaderId = resourceManager->compileShaderCode(
      InterpolatedVS, InterpolatedVS_size, VERTEX_SHADER);
  if (not vertexShaderId)
  {
    return false;
  }

  unsigned pixelShaderId = resourceManager->compileShaderCode(
      InterpolatedDepthPS, InterpolatedDepthPS_size, PIXEL_SHADER);
  if (not pixelShaderId)
  {
    return false;
  }

  unsigned programId = resourceManager->createShaderProgram(
      0, vertexShaderId, 0, 0, 0, pixelShaderId);
  if (not programId)
  {
    return false;
  }

  // MAYBE: might be better to fetch the depthTextureId here instead of using
  // constuctor
  // this->depthTextureId =
  //     (dynamic_cast<DepthShader*>(
  //          IShaderManager::getInstance()->getShader(DEPTH)))->getDepthTexture();

  this->setOutputId(0);

  this->setProgramId(programId);

  this->setDepthState(ZGE_DEPTH_ON);

  return true;
}

void PsychedaelicShader::prepare()
{
  MeshShader::prepare();

  IRenderer* renderer = IRenderer::getInstance();

  renderer->setTexture(0, this->depthTextureId, POINT_CLAMP, PIXEL_SHADER);
  // glm::mat4 viewMatrix = IGame::getInstance()
  //                            ->getCurrentGameState()
  //                            ->getCurrentCamera()
  //                            ->getViewMatrix();

  glm::mat4 viewMatrix[] = {
    IGame::getInstance()
        ->getCurrentGameState()
        ->getCurrentCamera()
        ->getViewMatrix(),
    IResourceManager::getInstance()->getPerspectiveMatrix()
  };
  renderer->setConstant(0, glm::value_ptr(*viewMatrix), sizeof(glm::mat4) * 2,
                        VERTEX_SHADER);
  // std::cout << "viewMatrix: " << glm::to_string(viewMatrix) << std::endl;
  // glm::mat4* view = new glm::mat4(viewMatrix);
  // renderer->setConstant(0, glm::value_ptr(viewMatrix), sizeof(glm::mat4),
  //                       VERTEX_SHADER);
  // set depth bool in pixel shader
  renderer->setConstant(1, &(this->showDepth), sizeof(bool), PIXEL_SHADER);
  // delete view;
}

void PsychedaelicShader::finish()
{
  // unbind the texture
  IRenderer* renderer = IRenderer::getInstance();
  renderer->setTexture(0, 0, 0, PIXEL_SHADER);

  ShaderProgram::finish();
}

void PsychedaelicShader::switchShowDepth()
{
  this->showDepth = not this->showDepth;
}

void PsychedaelicShader::addInstance(IGameObject* instance)
{
  MeshShader::addInstance(instance);
  IShaderManager::getInstance()->addInstance(SHADER_DEPTH, instance);
}

} // namespace zge
