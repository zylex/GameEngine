#include <algorithm>

#include "Preprocessors.h"

#include "MeshFactory.h"
#include "IResourceManager.h"

namespace zge
{

MeshFactory::MeshFactory()
{
  // constructor
}

MeshFactory::~MeshFactory() NOEXCEPT
{
  // destructor
}

MeshFactory::MeshFactory(const MeshFactory& other)
{
  // copy constructor
}

MeshFactory& MeshFactory::operator=(const MeshFactory& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

MeshFactory::MeshFactory(MeshFactory&& other) NOEXCEPT
{
  // move constructor (C++11)
}

MeshFactory& MeshFactory::operator=(MeshFactory&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new MeshFactory(other));
}

std::vector<glm::vec3> MeshFactory::getVertices() { return this->vertices; }

std::vector<glm::vec3> MeshFactory::getNormals() { return this->normals; }

std::vector<glm::uvec3> MeshFactory::getIndices() { return this->indices; }

void MeshFactory::generateSquareMesh()
{
  this->clear();
  this->vertices.push_back(glm::vec3{ -0.5f, -0.5f, 0.0f });
  this->vertices.push_back(glm::vec3{ 0.5f, -0.5f, 0.0f });
  this->vertices.push_back(glm::vec3{ 0.5f, 0.5f, 0.0f });
  this->vertices.push_back(glm::vec3{ -0.5f, 0.5f, 0.0f });
  this->indices = { { 0, 1, 2 }, { 2, 3, 0 } };
  this->generateNormals();
}

void MeshFactory::generateCylinderMesh()
{
  this->clear();
  // FIXME: Implement generateCylinderMesh
}

void MeshFactory::generateIcosphereMesh(const unsigned recursionLevel)
{

  this->clear();
  // http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
  // FIXME: Implement generateIcosphereMesh (should be copy/paste job)
}

void MeshFactory::clear()
{
  this->vertices.clear();
  this->normals.clear();
  this->indices.clear();
}

const unsigned MeshFactory::addVertex(glm::vec3 vertex)
{
  std::vector<glm::vec3>::iterator it =
      std::find(this->vertices.begin(), this->vertices.end(), vertex);
  if (it IS this->vertices.end())
  {
    this->vertices.push_back(vertex);
    it = std::find(this->vertices.begin(), this->vertices.end(), vertex);
  }
  return std::distance(this->vertices.begin(), it);
}

void MeshFactory::generateNormals()
{
  this->normals.resize(this->vertices.size());
  for (std::vector<glm::uvec3>::iterator i = this->indices.begin();
       i != this->indices.end(); ++i)
  {
    glm::vec3 vertex1 = this->vertices[i->x];
    glm::vec3 vertex2 = this->vertices[i->y];
    glm::vec3 vertex3 = this->vertices[i->z];
    glm::vec3 u = { vertex2.x - vertex1.x, vertex2.y - vertex1.y,
                    vertex2.z - vertex1.z };
    glm::vec3 v = { vertex3.x - vertex1.x, vertex3.y - vertex1.y,
                    vertex3.z - vertex1.z };
    float nx = u.y * v.z - u.z * v.y;
    float ny = u.z * v.x - u.x * v.z;
    float nz = u.x * v.y - u.y * v.x;
    this->normals[i->x] += glm::vec3{ nx, ny, nz };
    this->normals[i->y] += glm::vec3{ nx, ny, nz };
    this->normals[i->z] += glm::vec3{ nx, ny, nz };
  }

  // normalize
  for (std::vector<glm::vec3>::iterator i = this->normals.begin();
       i != this->normals.end(); ++i)
  {
    *i = glm::normalize(*i);
  }
}

} // namespace zge
