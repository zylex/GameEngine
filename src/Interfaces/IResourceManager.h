#ifndef IRESOURCEMANAGER_H
#define IRESOURCEMANAGER_H

#include <cstddef>
#include <string>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "Preprocessors.h"

namespace zge
{

class IResourceManager
{
public:
  static IResourceManager* getInstance();
  virtual ~IResourceManager() NOEXCEPT = default;

  virtual const unsigned createMeshFromFile(const std::string filePath) = 0;
  virtual const unsigned createMesh(const std::vector<glm::vec3>& vertices,
                                    const std::vector<glm::vec3>& normals,
                                    const std::vector<glm::uvec3>& indices) = 0;
  // virtual const unsigned createMesh(
  //     const std::vector<glm::vec3>&,       // vertices
  //     const std::vector<glm::uvec3>&) = 0; // indices
  // virtual const unsigned createInstanceBuffer(
  //     const std::vector<glm::mat4>&) = 0;
  virtual const unsigned createDepthBuffer(const unsigned textureId) = 0;

  virtual const unsigned getInstanceBuffer() = 0;
  virtual const unsigned updateInstanceBuffer(
      const std::vector<glm::mat4>& instanceData) = 0;

  virtual const unsigned getSquareMesh() = 0;
  // virtual void generateCylinderMesh() = 0;
  // virtual void generateIcosphereMesh(const unsigned) = 0;

  virtual const unsigned compileShaderCode(const void* shaderCode,
                                           const std::size_t shaderCodeSize,
                                           const unsigned shaderType) = 0;
  virtual const unsigned createShaderProgram(const unsigned computerShaderId,
                                             const unsigned vertexShaderId,
                                             const unsigned geometryShaderId,
                                             const unsigned hullShaderId,
                                             const unsigned domainShaderId,
                                             const unsigned pixelShaderId) = 0;

  virtual const unsigned createRawTexture2D(const unsigned width,
                                            const unsigned height,
                                            const unsigned textureType) = 0;
  virtual const unsigned createOutputBuffer(
      const unsigned depthTexture,
      const std::vector<unsigned> outputTextures) = 0;

  virtual const unsigned addMeshIndexCount(
      const std::pair<unsigned, unsigned> meshIndexCount) = 0;
  virtual std::pair<unsigned, unsigned> getIndexCount(
      const unsigned meshId) = 0;
};

} // namespace zge

#endif