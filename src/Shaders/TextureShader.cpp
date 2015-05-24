#ifdef DEBUG
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#endif

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SamplerType.h"
#include "ShaderType.h"
#include "DepthState.h"

#include "IGame.h"
#include "IRenderer.h"
#include "IResourceManager.h"
#include "IGameState.h"

#include "TextureShader.h"

#ifdef USE_ANT
#include "Stats.h"
#endif

#include "TexturedVS.h"
#include "TexturedPS.h"

#include "Camera.h"
#include "Light.h"

namespace zge
{

TextureShader::TextureShader()
{
  // constructor
}

TextureShader::~TextureShader() NOEXCEPT
{
  // destructor
  this->clear();
}

const glm::mat4 TextureShader::getInstanceData(
    const MeshObject* gameObject) const
{
  glm::vec3 rotation = gameObject->getRotation();
  glm::mat4 translationMatrix = glm::translate(
      glm::mat4(), gameObject->getOrigin() + gameObject->getPosition());
  glm::mat4 rotationMatrix =
      glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x),
                         glm::radians(rotation.z));
  glm::mat4 scalingMatrix = glm::scale(glm::mat4(), gameObject->getScale());

  glm::mat4 result = translationMatrix * rotationMatrix * scalingMatrix;

  return result;
}

const TextureShader::Instance TextureShader::createInstanceUnit(
    MeshObject* gameObject, const glm::mat4& instanceData) const
{
  Instance result;
  result.meshId = gameObject->getMeshId();
  result.textureId = gameObject->getMaterial()->getTextureId();
  result.instanceObjects.push_back(gameObject);
  result.instanceData.push_back(instanceData);
  return result;
}

void TextureShader::executeShader(const Instance& instance) const
{
  IRenderer* renderer = IRenderer::getInstance();
  renderer->setTexture(0, instance.textureId, LINEAR_WRAP, PIXEL_SHADER);
  renderer->executeInstancedShader(
      instance.meshId, instance.instanceData.data(), sizeof(glm::mat4),
      instance.instanceData.size());
}

TextureShader::InstanceFinder::InstanceFinder(MeshObject* gameObject)
    : object(gameObject)
{
}

const bool TextureShader::InstanceFinder::operator()(
    const Instance& instance) const
{
  return instance.textureId IS this->object->getMaterial()->getTextureId() and
         instance.meshId IS this->object->getMeshId();
}

TextureShader::ObjectFinder::ObjectFinder(MeshObject* gameObject)
    : instance(gameObject)
{
}

const bool TextureShader::ObjectFinder::operator()(
    const MeshObject* object) const
{
  return object->getMaterial() IS this->instance->getMaterial() and
         object->getMeshId() IS this->instance->getMeshId();
}

const bool TextureShader::InstanceSorter::operator()(const Instance& lhs,
                                                     const Instance& rhs)
{
  return lhs.textureId < rhs.textureId and lhs.meshId < rhs.meshId;
}

const bool TextureShader::initialise()
{
  this->clear();

  IResourceManager* resourceManager = IResourceManager::getInstance();
  unsigned vertexShaderId = resourceManager->compileShaderCode(
      TexturedVS, TexturedVS_size, VERTEX_SHADER);
  if (not vertexShaderId)
  {
    return false;
  }

  unsigned pixelShaderId = resourceManager->compileShaderCode(
      TexturedPS, TexturedPS_size, PIXEL_SHADER);
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

  this->setOutputId(0);

  this->setProgramId(programId);

  this->setDepthState(ZGE_DEPTH_ON);

  return true;
}

void TextureShader::prepare()
{
  ShaderProgram::prepare();
  IRenderer* renderer = IRenderer::getInstance();
  // renderer->enableAntiClockwiseCulling();
  renderer->disableCulling();
  renderer->disableAlphaBlending();
  // renderer->enableDepth();
  IGameState* gameState = IGame::getInstance()->getCurrentGameState();

  glm::mat4 viewMatrix[] = {
    gameState->getCurrentCamera()->getViewMatrix(),
    IResourceManager::getInstance()->getPerspectiveMatrix()
  };

  renderer->setConstant(0, glm::value_ptr(*viewMatrix), sizeof(glm::mat4) * 2,
                        VERTEX_SHADER);
  Light* light = gameState->getLights()[0];
  glm::vec4 diffuseColor = light->getDiffuseColor();
  glm::vec3 lightVector;
  LightType lightType = light->getLightType();
  float specularPower = light->getSpecularPower();
  if (light->getLightType() IS DIRECTIONAL_LIGHT)
  {
    lightVector = light->getLightDirection();
  }
  else
  {
    lightVector = light->getPosition();
  }
  this->lightData = {
    light->getAmbientColor(), light->getDiffuseColor(),
    light->getSpecularColor(), lightVector, specularPower,
    gameState->getCurrentCamera()->getPosition(), lightType,
  };
  renderer->setConstant(1, &this->lightData, sizeof(LightData), PIXEL_SHADER);
}

void TextureShader::clear() { this->instances.clear(); }

void TextureShader::addInstance(IGameObject* instance)
{
  instance->addInstance(this);
}

void TextureShader::addInstance(MeshObject* instance)
{
  std::vector<Instance>::iterator instanceBegin = this->instances.begin();
  std::vector<Instance>::iterator instanceEnd = this->instances.end();
  std::vector<Instance>::iterator instanceIterator =
      std::find_if(instanceBegin, instanceEnd, InstanceFinder(instance));
  if (instanceIterator IS instanceEnd) // not already there
  {
    this->instances.push_back(
        this->createInstanceUnit(instance, this->getInstanceData(instance)));
    instanceBegin = this->instances.begin();
    instanceEnd = this->instances.end();
    std::sort(instanceBegin, instanceEnd, InstanceSorter());
#ifdef USE_ANT
    ++Stats::numberOfInstances;
#endif
  }
  else
  {
    std::vector<MeshObject*> instanceObjects =
        instanceIterator->instanceObjects;
    std::vector<MeshObject*>::iterator instanceObjectsFound, instanceObjectsEnd;
    instanceObjectsEnd = instanceIterator->instanceObjects.end();
    instanceObjectsFound = std::find(instanceIterator->instanceObjects.begin(),
                                     instanceObjectsEnd, instance);
    if (instanceObjectsFound IS instanceObjectsEnd) // not already there
    {
      instanceIterator->instanceObjects.push_back(instance);
      instanceIterator->instanceData.push_back(this->getInstanceData(instance));
#ifdef USE_ANT
      ++Stats::numberOfInstances;
#endif
    }
    // else
    // {
    //   // do nothing
    // }
  }
}

void TextureShader::render()
{
  std::vector<Instance>::iterator instanceEnd = this->instances.end();
  std::vector<Instance>::iterator instanceIterator;
  for (instanceIterator = this->instances.begin();
       instanceIterator IS_NOT instanceEnd; ++instanceIterator)
  {
    this->updateData(instanceIterator->instanceObjects,
                     instanceIterator->instanceData);
    this->executeShader(*instanceIterator);
  }
}

void TextureShader::updateData(std::vector<MeshObject*>& gameObjects,
                               std::vector<glm::mat4>& instanceData)
{
  std::vector<MeshObject*>::iterator objectBegin, objectEnd, objectIterator;
  objectBegin = gameObjects.begin();
  objectEnd = gameObjects.end();
  for (objectIterator = objectBegin; objectIterator IS_NOT objectEnd;
       ++objectIterator)
  {
    if ((*objectIterator)->isDirty())
    {
      unsigned long index = std::distance(objectBegin, objectIterator);
      instanceData[index] = this->getInstanceData(*objectIterator);
    }
  }
}

} // namespace zge
