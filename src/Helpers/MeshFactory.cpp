#include <algorithm>

#include <glm/geometric.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
                     { -0.5f, 0.5f, 0.0f },
                     { 0.5f, 0.5f, 0.0f },
                     { 0.5f, -0.5f, 0.0f } };
  this->indices = { { 0, 1, 2 }, { 2, 3, 0 } };
  this->textureCoordinates = {
    { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }
  };
  this->generateNormals();

  this->generateTangents();
}

void MeshFactory::generateCylinderMesh()
{
  this->clear();
  // FIXME: Implement generateCylinderMesh

  this->generateTextureCoordinates();
  this->generateNormals();
  this->generateTangents();
}

void MeshFactory::generateIcosphereMesh(const unsigned recursionLevel)
{
  this->clear();

  this->generateIcosphereVerticesAndIndices(recursionLevel);

  this->generateTextureCoordinates();
  this->generateNormals();
  this->generateTangents();
}

void MeshFactory::generateSquareMeshWithHole()
{
  this->clear();

  this->vertices = { { -2.0f, -2.0f, 0.0f },       // 0 bottom left
                     { -2.0f, 2.0f, 0.0f },        // 1 top left
                     { 2.0f, 2.0f, 0.0f },         // 2 top right
                     { 2.0f, -2.0f, 0.0f } };      // 3 bottom right
  this->vertices.push_back({ 0.0f, -1.0f, 0.0f }); // 4 bottom
  this->vertices.push_back({ 0.0f, 1.0f, 0.0f });  // 5 top
  this->vertices.push_back({ 1.0f, 0.0f, 0.0f });  // 6 right
  this->vertices.push_back({ -1.0f, 0.0f, 0.0f }); // 7 left

  this->indices = { { 0, 1, 7 },
                    { 1, 2, 5 },
                    {
                     2, 3, 6,
                    },
                    { 3, 0, 4 } };

  glm::vec3 left = { -1.0f, 0.0f, 0.0f };
  glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
  // top left corner
  long previousIndex = 7;
  for (int i = 0; i < 8; ++i)
  {
    rotation.z += 10.0f;
    glm::vec3 vertex =
        left * glm::mat3(glm::eulerAngleYXZ(glm::radians(rotation.y),
                                            glm::radians(rotation.x),
                                            glm::radians(rotation.z)));
    this->vertices.push_back(glm::normalize(vertex));
    this->indices.push_back({ previousIndex, 1, this->vertices.size() - 1 });
    previousIndex = this->vertices.size() - 1;
  }
  this->indices.push_back({ this->vertices.size() - 1, 1, 5 });

  // top right corner
  rotation.z = 90.0f;
  previousIndex = 5;
  for (int i = 0; i < 8; ++i)
  {
    glm::vec3 vertex =
        left * glm::mat3(glm::eulerAngleYXZ(glm::radians(rotation.y),
                                            glm::radians(rotation.x),
                                            glm::radians(rotation.z)));
    this->vertices.push_back(glm::normalize(vertex));
    rotation.z += 10.0f;
    this->indices.push_back({ previousIndex, 2, this->vertices.size() - 1 });
    previousIndex = this->vertices.size() - 1;
  }
  this->indices.push_back({ this->vertices.size() - 1, 2, 6 });

  // bottom right corner
  rotation.z = 180.0f;
  previousIndex = 6;
  for (int i = 0; i < 8; ++i)
  {
    glm::vec3 vertex =
        left * glm::mat3(glm::eulerAngleYXZ(glm::radians(rotation.y),
                                            glm::radians(rotation.x),
                                            glm::radians(rotation.z)));
    this->vertices.push_back(glm::normalize(vertex));
    rotation.z += 10.0f;
    this->indices.push_back({ previousIndex, 3, this->vertices.size() - 1 });
    previousIndex = this->vertices.size() - 1;
  }
  this->indices.push_back({ this->vertices.size() - 1, 3, 4 });

  // bottom left corner
  rotation.z = -90.0f;
  previousIndex = 4;
  for (int i = 0; i < 8; ++i)
  {
    glm::vec3 vertex =
        left * glm::mat3(glm::eulerAngleYXZ(glm::radians(rotation.y),
                                            glm::radians(rotation.x),
                                            glm::radians(rotation.z)));
    this->vertices.push_back(glm::normalize(vertex));
    rotation.z += 10.0f;
    this->indices.push_back({ previousIndex, 0, this->vertices.size() - 1 });
    previousIndex = this->vertices.size() - 1;
  }
  this->indices.push_back({ this->vertices.size() - 1, 0, 7 });

  this->generateTextureCoordinates();
  this->generateNormals();

  for (int i = 0; i < this->normals.size(); ++i)
  {
    this->normals[i] = -this->normals[i];
  }

  this->generateTangents();
}

void MeshFactory::generateSemiSphereMesh()
{
  this->clear();

  this->generateSphereVerticesAndIndices();

  for (std::vector<glm::vec3>::iterator i = this->vertices.begin();
       i IS_NOT this->vertices.end(); ++i)
  {
    if (i->z > 0.0f)
    {
      i->z = -i->z + 0.001f;
    }
  }

  this->generateTextureCoordinates();
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
       i IS_NOT this->indices.end(); ++i)
  {
    glm::vec3 vertex1 = this->vertices[i->x];
    glm::vec3 vertex2 = this->vertices[i->y];
    glm::vec3 vertex3 = this->vertices[i->z];

    glm::vec3 normal = -glm::triangleNormal(vertex1, vertex2, vertex3);

    this->normals[i->x] += normal;
    this->normals[i->y] += normal;
    this->normals[i->z] += normal;
  }

  // normalize
  for (std::vector<glm::vec3>::iterator i = this->normals.begin();
       i IS_NOT this->normals.end(); ++i)
  {
    *i = glm::normalize(*i);
    // *i = -glm::normalize(*i);
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

void MeshFactory::generateTextureCoordinates()
{
  unsigned vertexCount = this->vertices.size();
  this->textureCoordinates.resize(vertexCount);
  for (int i = 0; i < vertexCount; ++i)
  {
    glm::vec3 vertex = this->vertices[i];
    if (vertex.z < -0.001f)
    {
      vertex = -vertex;
    }
    this->textureCoordinates[i] = { vertex.x, vertex.y };
  }
}

void MeshFactory::generateIcosphereVerticesAndIndices(
    const unsigned recursionLevel)
{
  // http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html

  float t = (1.0f + sqrtf(5.0f)) / 2.0f;
  this->vertices.push_back(glm::normalize(glm::vec3{ -1.0f, t, 0.0f }));
  this->vertices.push_back(glm::normalize(glm::vec3{ 1.0f, t, 0.0f }));
  this->vertices.push_back(glm::normalize(glm::vec3{ -1.0f, -t, 0.0f }));
  this->vertices.push_back(glm::normalize(glm::vec3{ 1.0f, -t, 0.0f }));

  this->vertices.push_back(glm::normalize(glm::vec3{ 0.0f, -1.0f, t }));
  this->vertices.push_back(glm::normalize(glm::vec3{ 0.0f, 1.0f, t }));
  this->vertices.push_back(glm::normalize(glm::vec3{ 0.0f, -1.0f, -t }));
  this->vertices.push_back(glm::normalize(glm::vec3{ 0.0f, 1.0f, -t }));

  this->vertices.push_back(glm::normalize(glm::vec3{ t, 0.0f, -1.0f }));
  this->vertices.push_back(glm::normalize(glm::vec3{ t, 0.0f, 1.0f }));
  this->vertices.push_back(glm::normalize(glm::vec3{ -t, 0.0f, -1.0f }));
  this->vertices.push_back(glm::normalize(glm::vec3{ -t, 0.0f, 1.0f }));

  this->indices.push_back({ 0, 11, 5 });
  this->indices.push_back({ 0, 5, 1 });
  this->indices.push_back({ 0, 1, 7 });
  this->indices.push_back({ 0, 7, 10 });
  this->indices.push_back({ 0, 10, 11 });

  this->indices.push_back({ 1, 5, 9 });
  this->indices.push_back({ 5, 11, 4 });
  this->indices.push_back({ 11, 10, 2 });
  this->indices.push_back({ 10, 7, 6 });
  this->indices.push_back({ 7, 1, 8 });

  this->indices.push_back({ 3, 9, 4 });
  this->indices.push_back({ 3, 4, 2 });
  this->indices.push_back({ 3, 2, 6 });
  this->indices.push_back({ 3, 6, 8 });
  this->indices.push_back({ 3, 8, 9 });

  this->indices.push_back({ 4, 9, 5 });
  this->indices.push_back({ 2, 4, 11 });
  this->indices.push_back({ 6, 2, 10 });
  this->indices.push_back({ 8, 6, 7 });
  this->indices.push_back({ 9, 8, 1 });

  for (int i = 0; i < recursionLevel; ++i)
  {
    std::vector<glm::uvec3> faces;

    for (std::vector<glm::uvec3>::iterator it = this->indices.begin();
         it IS_NOT this->indices.end(); ++it)
    {
      glm::vec3 p1p2 = (this->vertices[it->x] + this->vertices[it->y]) / 2.0f;
      glm::vec3 p2p3 = (this->vertices[it->y] + this->vertices[it->z]) / 2.0f;
      glm::vec3 p3p1 = (this->vertices[it->z] + this->vertices[it->x]) / 2.0f;
      long a = this->vertices.size();
      this->vertices.push_back(glm::normalize(p1p2));
      long b = this->vertices.size();
      this->vertices.push_back(glm::normalize(p2p3));
      long c = this->vertices.size();
      this->vertices.push_back(glm::normalize(p3p1));

      faces.push_back({ it->x, a, c });
      faces.push_back({ it->y, b, a });
      faces.push_back({ it->z, c, b });
      faces.push_back({ a, b, c });
    }
    this->indices = faces;
  }
}

void MeshFactory::generateSphereVerticesAndIndices()
{
  // TODO:
  std::vector<unsigned> currentLine, previousLine, firstLine;
  glm::vec3 left = { -1.0f, 0.0f, 0.0f };
  glm::vec3 vertex = glm::normalize(glm::vec3{ -1.0f, 0.0f, 0.0f });
  // currentLine.push_back(this->vertices.size());
  this->vertices.push_back(vertex);
  // this->indices.push_back({ 0, 1, 2 });

  glm::mat3 rotationMatrix = glm::mat3(glm::eulerAngleYXZ(
      glm::radians(0.0f), glm::radians(0.0f), glm::radians(4.0f)));

  for (int i = 1; i < 44; ++i)
  {
    // vertex = glm::normalize(glm::rotate(vertex, 10.0f, { 0.0f, 0.0f, 1.0f
    // }));
    vertex = glm::normalize(vertex * rotationMatrix);
    currentLine.push_back(this->vertices.size());
    this->vertices.push_back(vertex);
    // this->vertices.push_back({ 0.0f, 0.0f, 0.0f });
  }
  currentLine.push_back(this->vertices.size());
  this->vertices.push_back(glm::normalize(glm::vec3{ 1.0f, 0.0f, 0.0f }));

  firstLine = currentLine;

  rotationMatrix = glm::mat3(glm::eulerAngleYXZ(
      glm::radians(0.0f), glm::radians(4.0f), glm::radians(0.0f)));

  for (int i = 1; i < 89; ++i)
  {
    previousLine = currentLine;
    currentLine.clear();
    currentLine.push_back(previousLine[0]);
    // int j = 0;
    for (int j = 1; j < previousLine.size() - 1; ++j)
    {
      vertex = this->vertices[previousLine[j]];
      vertex = glm::normalize(vertex * rotationMatrix);
      currentLine.push_back(this->vertices.size());
      this->vertices.push_back(vertex);
    }
    currentLine.push_back(previousLine[previousLine.size() - 1]);
    // }

    for (int j = 0; j < currentLine.size() - 1; ++j)
    {
      unsigned a, b, c, d;
      a = previousLine[j];
      b = previousLine[j + 1];
      c = currentLine[j + 1];
      d = currentLine[j];

      this->indices.push_back({ a, b, c });
      this->indices.push_back({ c, d, a });
    }
  }

  for (int j = 0; j < currentLine.size() - 1; ++j)
  {
    unsigned a, b, c, d;
    a = currentLine[j];
    b = currentLine[j + 1];
    c = firstLine[j + 1];
    d = firstLine[j];

    this->indices.push_back({ a, b, c });
    this->indices.push_back({ c, d, a });
  }
}

} // namespace zge
