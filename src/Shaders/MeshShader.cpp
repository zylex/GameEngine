#include <iostream>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
// #if defined(_DEBUG) || defined(DEBUG) || defined(NDEBUG)
// #include <glm/gtx/string_cast.hpp>
// #endif

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
  // std::cout << "translationMatrix" << glm::to_string(translationMatrix)
  // << std::endl;
  glm::mat4 rotationMatrix =
      glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
  // std::cout << "rotationMatrix" << glm::to_string(rotationMatrix) <<
  // std::endl;
  glm::mat4 scalingMatrix = glm::scale(glm::mat4(), gameObject->getScale());
  // std::cout << "scalingMatrix" << glm::to_string(scalingMatrix) << std::endl;
  //     glm::scale(glm::translate(glm::mat4(), gameObject->getOrigin() +
  // glm::mat4 result = glm::transpose(
  //                                                gameObject->getPosition()) *
  //                    glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z),
  //                gameObject->getScale()));

  glm::mat4 result = translationMatrix * rotationMatrix * scalingMatrix;
  result = glm::transpose(result);
  // std::cout << "result" << glm::to_string(result) << std::endl;

  return result;
}

const Instance MeshShader::getInstanceUnit(MeshObject* gameObject,
                                           const glm::mat4& instanceData) const
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

  // #if defined(_DEBUG) || defined(DEBUG) || defined(NDEBUG)
  //   std::cout << std::endl;
  //   std::cout << "Frame instance data from MeshShader:" << std::endl;
  //   for (std::vector<glm::mat4>::const_iterator i =
  //   instance.instanceData.begin();
  //        i != instance.instanceData.end(); ++i)
  //   {
  //     std::cout << glm::to_string(*i) << std::endl;
  //   }
  //   std::cout << std::endl;
  //   std::cout << std::endl;
  // #endif
}

MeshShaderStructs::InstanceFinder::InstanceFinder(MeshObject* gameObject)
    : instance(gameObject)
{
}

bool MeshShaderStructs::InstanceFinder::operator()(
    const Instance& gameObject) const
{
  return gameObject.meshId IS this->instance->getMeshId();
}

bool MeshShaderStructs::InstanceSorter::operator()(const Instance& lhs,
                                                   const Instance& rhs)
{
  return lhs.meshId < rhs.meshId;
}

} // namespace zge
