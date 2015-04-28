#include <iostream>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "SamplerType.h"
#include "IRenderer.h"

#include "MeshShader.h"

namespace zge
{

MeshShader::MeshShader()
{
  // constructor
}

MeshShader::~MeshShader() NOEXCEPT
{
  // destructor
  this->clear();
}

const glm::mat4 MeshShader::getInstanceData(const MeshObject* gameObject) const
{
  glm::vec3 rotation = gameObject->getRotation();
  glm::mat4 translationMatrix = glm::translate(
      glm::mat4(), gameObject->getOrigin() + gameObject->getPosition());
  glm::mat4 rotationMatrix =
      glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
  glm::mat4 scalingMatrix = glm::scale(glm::mat4(), gameObject->getScale());

  glm::mat4 result = translationMatrix * rotationMatrix * scalingMatrix;
  result = glm::transpose(result);

  return result;
}

const MeshShader::Instance MeshShader::createInstanceUnit(
    MeshObject* gameObject, const glm::mat4& instanceData) const
{
  Instance result;
  result.meshId = gameObject->getMeshId();
  result.instanceObjects.push_back(gameObject);
  result.instanceData.push_back(instanceData);
  return result;
}

void MeshShader::executeShader(const Instance& instance) const
{
  IRenderer::getInstance()->executeInstancedShader(
      instance.meshId, instance.instanceData.data(), sizeof(glm::mat4),
      instance.instanceData.size());
}

MeshShader::InstanceFinder::InstanceFinder(MeshObject* gameObject)
    : instance(gameObject)
{
}

const bool MeshShader::InstanceFinder::operator()(
    const Instance& gameObject) const
{
  return gameObject.meshId IS this->instance->getMeshId();
}

MeshShader::ObjectFinder::ObjectFinder(MeshObject* gameObject)
    : instance(gameObject)
{
}

const bool MeshShader::ObjectFinder::operator()(const MeshObject* object) const
{
  return object->getMeshId() IS this->instance->getMeshId();
}

const bool MeshShader::InstanceSorter::operator()(const Instance& lhs,
                                                  const Instance& rhs)
{
  return lhs.meshId < rhs.meshId;
}

void MeshShader::prepare() { ShaderProgram::prepare(); }

void MeshShader::clear() { this->instances.clear(); }

void MeshShader::addInstance(IGameObject* instance)
{
  instance->addInstance(this);
}

void MeshShader::addInstance(MeshObject* instance)
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
  }
  else
  {
    std::vector<MeshObject*> instanceObjects =
        instanceIterator->instanceObjects;
    std::vector<MeshObject*>::iterator instanceObjectsFound, instanceObjectsEnd;
    instanceObjectsEnd = instanceObjects.end();
    instanceObjectsFound =
        std::find(instanceObjects.begin(), instanceObjectsEnd, instance);
    if (instanceObjectsFound IS instanceObjectsEnd) // not already there
    {
      instanceIterator->instanceObjects.push_back(instance);
      instanceIterator->instanceData.push_back(this->getInstanceData(instance));
    }
    // else
    // {
    //   // do nothing
    // }
  }
}

void MeshShader::render()
{
  // TODO: implement
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

void MeshShader::updateData(std::vector<MeshObject*>& objectPairs,
                            std::vector<glm::mat4>& instanceData)
{
  std::vector<MeshObject*>::iterator objectBegin, objectEnd, objectIterator;
  objectBegin = objectPairs.begin();
  objectEnd = objectPairs.end();
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
