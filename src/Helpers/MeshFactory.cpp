#include <algorithm>

#include <glm/geometric.hpp>
#include <glm/gtx/normal.hpp>

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

std::vector<glm::vec2> MeshFactory::getTextureCoordinates()
{
  return this->textureCoordinates;
}

std::vector<glm::vec3> MeshFactory::getNormals() { return this->normals; }

std::vector<glm::uvec3> MeshFactory::getIndices() { return this->indices; }

void MeshFactory::generateSquareMesh()
{
  this->clear();
  this->vertices = { { -0.5f, -0.5f, 0.0f },
                     { 0.5f, -0.5f, 0.0f },
                     { 0.5f, 0.5f, 0.0f },
                     { -0.5f, 0.5f, 0.0f } };
  this->indices = { { 0, 1, 2 }, { 2, 3, 0 } };
  this->textureCoordinates = {
    { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
  };
  this->generateNormals();
  this->generateTangents();
}

void MeshFactory::generateCylinderMesh()
{
  this->clear();
  // FIXME: Implement generateCylinderMesh

  this->generateNormals();
  this->generateTangents();
}

void MeshFactory::generateIcosphereMesh(const unsigned recursionLevel)
{

  this->clear();
  // http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
  // FIXME: Implement generateIcosphereMesh (should be copy/paste job from old
  // code)

  this->generateNormals();
  this->generateTangents();
}

void MeshFactory::clear()
{
  this->vertices.clear();
  this->normals.clear();
  this->indices.clear();
  this->tangents.clear();
  this->bitangents.clear();
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

    glm::vec3 normal = glm::triangleNormal(vertex1, vertex2, vertex3);

    this->normals[i->x] += normal;
    this->normals[i->y] += normal;
    this->normals[i->z] += normal;
  }

  // normalize
  for (std::vector<glm::vec3>::iterator i = this->normals.begin();
       i != this->normals.end(); ++i)
  {
    *i = glm::normalize(*i);
  }
}
void MeshFactory::generateTangents()
{
  // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/#Computing_the_tangents_and_bitangents
  unsigned vertexCount = this->vertices.size();
  this->tangents.resize(vertexCount);
  this->bitangents.resize(vertexCount);

  for (std::vector<glm::uvec3>::iterator i = this->indices.begin();
       i IS_NOT this->indices.end(); ++i)
  {
    glm::vec3 vertex0 = this->vertices[i->x];
    glm::vec3 vertex1 = this->vertices[i->y];
    glm::vec3 vertex2 = this->vertices[i->z];

    glm::vec2 textureCoordinate0 = this->textureCoordinates[i->x];
    glm::vec2 textureCoordinate1 = this->textureCoordinates[i->y];
    glm::vec2 textureCoordinate2 = this->textureCoordinates[i->z];

    glm::vec3 deltaPosition1 = vertex1 - vertex0;
    glm::vec3 deltaPosition2 = vertex2 - vertex0;

    glm::vec2 deltaTexCoord1 = textureCoordinate1 - textureCoordinate0;
    glm::vec2 deltaTexCoord2 = textureCoordinate2 - textureCoordinate0;

    float r = 1.0f / (deltaTexCoord1.x * deltaTexCoord2.y -
                      deltaTexCoord1.y * deltaTexCoord2.x);

    glm::vec3 tangent = (deltaPosition1 * deltaTexCoord2.y -
                         deltaPosition2 * deltaTexCoord1.y) *
                        r;

    this->tangents[i->x] += tangent;
    this->tangents[i->y] += tangent;
    this->tangents[i->z] += tangent;

    this->bitangents[i->x] += glm::cross(tangent, this->normals[i->x]);
    this->bitangents[i->y] += glm::cross(tangent, this->normals[i->y]);
    this->bitangents[i->z] += glm::cross(tangent, this->normals[i->z]);
  }
}

} // namespace zge
