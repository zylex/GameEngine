#include "MeshObject.h"

#include "IShaderProgram.h"

namespace zge
{

MeshObject::MeshObject()
{
  // constructor
}

MeshObject::~MeshObject() NOEXCEPT
{
  // destructor
}

MeshObject::MeshObject(const MeshObject& other)
{
  // copy constructor
}

MeshObject& MeshObject::operator=(const MeshObject& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

MeshObject::MeshObject(MeshObject&& other) NOEXCEPT
{
  // move constructor (C++11)
}

MeshObject& MeshObject::operator=(MeshObject&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new MeshObject(other));
}

void MeshObject::update() {}

void MeshObject::draw() {}

void MeshObject::setMeshId(const unsigned meshId) { m_meshId = meshId; }

unsigned MeshObject::getMeshId() const { return m_meshId; }

void MeshObject::setMaterial(IMaterial* material) { m_material = material; }

IMaterial* MeshObject::getMaterial() const { return m_material; }

void MeshObject::addInstance(IShaderProgram* shader)
{
  shader->addInstance(this);
}

} // namespace zge
