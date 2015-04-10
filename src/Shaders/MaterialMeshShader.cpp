#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "SamplerType.h"
#include "ShaderType.h"
#include "IRenderer.h"

#include "MaterialMeshShader.h"

namespace zge
{

MaterialMeshShader::MaterialMeshShader()
{
  // constructor
}

MaterialMeshShader::~MaterialMeshShader() NOEXCEPT
{
  // destructor
  this->clear();
}

void MaterialMeshShader::executeShader(const Instance& instance) const
{
  IRenderer* renderer = IRenderer::getInstance();
  renderer->setTexture(0, instance.materialId, LINEAR_WRAP, PIXEL_SHADER);
  renderer->executeInstancedShader(
      instance.meshId, instance.instanceData.data(), sizeof(glm::mat4),
      instance.instanceData.size());
}

const InstanceData MaterialMeshShader::getInstanceData(
    const MeshObject* gameObject) const
{
  InstanceData result;
  glm::vec3 rotation = gameObject->getRotation();
  result.instanceMatrix = glm::transpose(
      glm::scale(glm::translate(glm::mat4(), gameObject->getOrigin() +
                                                 gameObject->getPosition()) *
                     glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z),
                 gameObject->getScale()));
  result.shininess = gameObject->getMaterial()->getShininess();
  return result;
}

const Instance MaterialMeshShader::getInstanceUnit(
    MeshObject* gameObject, const InstanceData& instanceData) const
{
  Instance result;
  result.materialId = gameObject->getMaterialId();
  result.meshId = gameObject->getMeshId();
  result.instanceObjects.push_back(gameObject);
  result.instanceData.push_back(instanceData);
  return result;
}

MaterialMeshShaderStructs::InstanceFinder::InstanceFinder(
    MeshObject* gameObject)
    : instance(gameObject)
{
}

bool MaterialMeshShaderStructs::InstanceFinder::operator()(
    const Instance& gameObject) const
{
  return gameObject.materialId IS this->instance->getMaterialId() and
         gameObject.meshId IS this->instance->getMeshId();
}

bool MaterialMeshShaderStructs::InstanceSorter::operator()(const Instance& lhs,
                                                           const Instance& rhs)
{
  return lhs.materialId < rhs.materialId or
         (lhs.materialId IS rhs.materialId and lhs.meshId < rhs.meshId);
}

} // namespace zge
