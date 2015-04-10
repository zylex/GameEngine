#ifdef DIRECT_X
#include "DirectXResourceManager.h"

namespace zge
{

IResourceManager* IResourceManager::getInstance()
{
  return dx::DirectXResourceManager::getInstance();
}

namespace dx
{

DirectXResourceManager* DirectXResourceManager::getInstance()
{
  static DirectXResourceManager instance;
  return &instance;
}

DirectXResourceManager::DirectXResourceManager()
{
  // constructor
}

DirectXResourceManager::~DirectXResourceManager() NOEXCEPT
{
  // destructor
}

DirectXResourceManager::DirectXResourceManager(
    const DirectXResourceManager& other)
{
  // copy constructor
}

DirectXResourceManager& DirectXResourceManager::operator=(
    const DirectXResourceManager& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

DirectXResourceManager::DirectXResourceManager(
    DirectXResourceManager&& other) NOEXCEPT
{
  // move constructor (C++11)
}

DirectXResourceManager& DirectXResourceManager::operator=(
    DirectXResourceManager&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new DirectXResourceManager(other));
}

const unsigned DirectXResourceManager::createMesh(
    const std::vector<glm::vec3> vertices,
    const std::vector<unsigned> indices) const
{
  // FIXME: implement createMesh
  return 0;
}

const unsigned DirectXResourceManager::createInstanceBuffer(
    const std::vector<glm::mat4> instancePositions) const
{
  // FIXME: implement createInstanceBuffer
  return 0;
}

const unsigned DirectXResourceManager::createDepthBuffer(
    const unsigned textureId) const
{
  // FIXME: implement createDepthBuffer
  return 0;
}

} // namespace dx

} // namespace zge

#endif