#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Preprocessors.h"

namespace zge
{

class MeshFactory
{
private:
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> textureCoordinates;
  std::vector<glm::vec3> normals;
  std::vector<glm::uvec3> indices;
  std::vector<glm::vec3> tangents;
  std::vector<glm::vec3> bitangents;
  // MAYBE: Tangents/Bi-tangents?

public:
  // constructor
  MeshFactory();
  // destructor
  virtual ~MeshFactory() NOEXCEPT;
  // copy constructor
  MeshFactory(const MeshFactory&);
  // assignment operator
  MeshFactory& operator=(const MeshFactory&);
  // move constructor (C++11)
  MeshFactory(MeshFactory&&) NOEXCEPT;
  // move assignment operator (C++11)
  MeshFactory& operator=(MeshFactory&&) NOEXCEPT;

  std::vector<glm::vec3> getVertices();
  std::vector<glm::vec3> getNormals();
  std::vector<glm::uvec3> getIndices();

  void generateSquareMesh();
  void generateCylinderMesh();
  void generateIcosphereMesh(const unsigned);

private:
  void clear();
  const unsigned addVertex(glm::vec3);
  void generateNormals();
  void generateTangents();
};

} // namespace zge

#endif
