#include "ModelFactory.h"

#include "IResourceManager.h"

#include "MeshFactory.h"
#include "CompositeObject.h"
#include "MeshObject.h"

namespace zge
{

ModelFactory::ModelFactory()
{
  // constructor
}

ModelFactory::~ModelFactory() NOEXCEPT
{
  // destructor
}

ModelFactory::ModelFactory(const ModelFactory& other)
{
  // copy constructor
}

ModelFactory& ModelFactory::operator=(const ModelFactory& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

ModelFactory::ModelFactory(ModelFactory&& other) NOEXCEPT
{
  // move constructor (C++11)
}

ModelFactory& ModelFactory::operator=(ModelFactory&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new ModelFactory(other));
}

IGameObject* ModelFactory::createBoxObject(float size)
{

  CompositeObject* result = new CompositeObject;

  MeshFactory meshFactory;
  meshFactory.generateSquareMesh();

  unsigned meshId = IResourceManager::getInstance()->createMesh(
      meshFactory.getVertices(), meshFactory.getNormals(),
      meshFactory.getIndices());

  std::vector<IGameObject*> children;

  for (int i = 0; i < 6; ++i)
  {
    MeshObject* meshObject = new MeshObject;
    meshObject->setMeshId(meshId);
    children.push_back(meshObject);
  }

  // front
  children[0]->setPosition({ 0.0f, 0.0f, 0.5f });
  children[0]->setRotation({ 0.0f, 0.0f, 0.0f });

  // back
  children[1]->setPosition({ 0.0f, 0.0f, -0.5f });
  children[1]->setRotation({ 0.0f, 180.0f, 0.0f });

  // left side
  children[2]->setPosition({ 0.0f, 0.5f, 0.0f });
  children[2]->setRotation({ 0.0f, -90.0f, 0.0f });

  // right side
  children[3]->setPosition({ 0.0f, -0.5f, 0.0f });
  children[3]->setRotation({ 0.0f, 90.0f, 0.0f });

  // top side
  children[4]->setPosition({ -0.5f, 0.0f, 0.0f });
  children[4]->setRotation({ 90.0f, 0.0f, 0.0f });

  // bottom side
  children[5]->setPosition({ 0.5f, 0.0f, 0.0f });
  children[5]->setRotation({ -90.0f, 0.0f, 0.0f });

  result->setChildren(children);

  result->setScale({ size, size, size });

  return result;
}

} // namespace zge
