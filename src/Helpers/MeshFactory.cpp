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

std::vector<glm::vec3> MeshFactory::getVertices() { return m_vertices; }

std::vector<glm::vec3> MeshFactory::getNormals() { return m_normals; }

std::vector<glm::uvec3> MeshFactory::getIndices() { return m_indices; }

void MeshFactory::generateSquareMesh()
{
  clear();
  m_vertices.push_back(glm::vec3{ -0.5f, -0.5f, 0.0f });
  m_vertices.push_back(glm::vec3{ 0.5f, -0.5f, 0.0f });
  m_vertices.push_back(glm::vec3{ 0.5f, 0.5f, 0.0f });
  m_vertices.push_back(glm::vec3{ -0.5f, 0.5f, 0.0f });
  m_indices = { { 0, 1, 2 }, { 2, 3, 0 } };
  generateNormals();
}

void MeshFactory::generateCylinderMesh()
{
  clear();
  // FIXME: Implement generateCylinderMesh
}

void MeshFactory::generateIcosphereMesh(const unsigned recursionLevel)
{

  clear();
  // http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
  // FIXME: Implement generateIcosphereMesh (should be copy/paste job)
}

void MeshFactory::clear()
{
  m_vertices.clear();
  m_normals.clear();
  m_indices.clear();
}

const unsigned MeshFactory::addVertex(glm::vec3 vertex)
{
  std::vector<glm::vec3>::iterator it =
      std::find(m_vertices.begin(), m_vertices.end(), vertex);
  if (it IS m_vertices.end())
  {
    m_vertices.push_back(vertex);
    it = std::find(m_vertices.begin(), m_vertices.end(), vertex);
  }
  return std::distance(m_vertices.begin(), it);
}

void MeshFactory::generateNormals()
{
  m_normals.resize(m_vertices.size());
  for (std::vector<glm::uvec3>::iterator i = m_indices.begin();
       i != m_indices.end(); ++i)
  {
    glm::vec3 vertex1 = m_vertices[i->x];
    glm::vec3 vertex2 = m_vertices[i->y];
    glm::vec3 vertex3 = m_vertices[i->z];
    glm::vec3 u = { vertex2.x - vertex1.x, vertex2.y - vertex1.y,
                    vertex2.z - vertex1.z };
    glm::vec3 v = { vertex3.x - vertex1.x, vertex3.y - vertex1.y,
                    vertex3.z - vertex1.z };
    float nx = u.y * v.z - u.z * v.y;
    float ny = u.z * v.x - u.x * v.z;
    float nz = u.x * v.y - u.y * v.x;
    m_normals[i->x] += glm::vec3{ nx, ny, nz };
    m_normals[i->y] += glm::vec3{ nx, ny, nz };
    m_normals[i->z] += glm::vec3{ nx, ny, nz };
  }

  // normalize
  for (std::vector<glm::vec3>::iterator i = m_normals.begin();
       i != m_normals.end(); ++i)
  {
    *i = glm::normalize(*i);
  }
}

} // namespace zge
