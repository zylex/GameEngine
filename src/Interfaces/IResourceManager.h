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
struct ShaderInputElement;

class IResourceManager
{
public:
  static IResourceManager* getInstance();
  virtual ~IResourceManager() NOEXCEPT = default;

  virtual const bool initialise() = 0;

  virtual const unsigned createMeshFromFile(const std::string filePath) = 0;
  virtual const unsigned createMesh(const std::vector<glm::vec3>& vertices,
                                    const std::vector<glm::vec2>& texCoords,
                                    const std::vector<glm::vec3>& normals,
                                    const std::vector<glm::uvec3>& indices) = 0;
  // TODO: implement createMesh with tangents for opengl and directx
  virtual const unsigned createMesh(
      const std::vector<glm::vec3>& /*vertices*/,
      const std::vector<glm::vec3>& /*normals*/,
      const std::vector<glm::uvec3>& /*indices*/,
      const std::vector<glm::vec2>& /*textureCoordinates*/,
      const std::vector<glm::vec3>& /*tangents*/,
      const std::vector<glm::vec3>& /*bitangents*/) = 0;

  virtual const unsigned getInstanceBuffer() = 0;

  virtual const unsigned compileShaderCode(const void* shaderCode,
                                           const std::size_t shaderCodeSize,
                                           const unsigned shaderType) = 0;
  virtual const unsigned createShaderProgram(const unsigned computeShaderId,
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
      const std::vector<unsigned>& outputTextures) = 0;

  virtual std::vector<unsigned>* getUniformBuffers(
      const unsigned programId) = 0;

  virtual const glm::mat4& getIdentityMatrix() const = 0;
  virtual const glm::mat4& getPerspectiveMatrix() const = 0;
  virtual const glm::mat4& getOrthographicMatrix() const = 0;

  virtual const unsigned getSquareMesh() = 0;
  virtual const unsigned getSquareMeshWithHole() = 0;
  virtual const unsigned getIcosphereMesh(const unsigned recursionLevel) = 0;
  virtual const unsigned getBowlMesh(const float radius,
                                     const float height) = 0;

  virtual const unsigned loadTextureFromFile(const std::string filepath) = 0;
};

} // namespace zge

#endif
