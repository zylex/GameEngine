#ifndef OPENGLRESOURCEMANAGER_H
#define OPENGLRESOURCEMANAGER_H

#include <glm/vec3.hpp>

#include "SamplerType.h"

#include "ResourceManager.h"

namespace zge
{
namespace gl
{

class OpenGLResourceManager : public ResourceManager
{
private:
  unsigned samplers[TOTAL_SAMPLERS];
  std::unordered_map<unsigned, std::vector<unsigned> > uniformBuffers;
  std::vector<std::pair<unsigned, unsigned> > meshIndexCounts;

  std::vector<unsigned> existingOutputs;
  std::vector<unsigned> existingTextures;

public:
  static OpenGLResourceManager* getInstance();
  // destructor
  virtual ~OpenGLResourceManager() noexcept;
  // copy constructor
  OpenGLResourceManager(const OpenGLResourceManager& other);
  // assignment operator
  OpenGLResourceManager& operator=(const OpenGLResourceManager& other);
  // move constructor (C++11)
  OpenGLResourceManager(OpenGLResourceManager&& other) noexcept;
  // move assignment operator (C++11)
  OpenGLResourceManager& operator=(OpenGLResourceManager&& other) noexcept;

  const unsigned createMesh(const std::vector<glm::vec3>& /*vertices*/,
                            const std::vector<glm::vec3>& /*normals*/,
                            const std::vector<glm::uvec3>& /*indices*/);
  const unsigned createMesh(
      const std::vector<glm::vec3>& /*vertices*/,
      const std::vector<glm::vec3>& /*normals*/,
      const std::vector<glm::uvec3>& /*indices*/,
      const std::vector<glm::vec2>& /*textureCoordinates*/,
      const std::vector<glm::vec3>& /*tangents*/,
      const std::vector<glm::vec3>& /*bitangents*/);

  const unsigned getInstanceBuffer();

  const unsigned compileShaderCode(const void*, const std::size_t,
                                   const unsigned);
  const unsigned createShaderProgram(const unsigned, const unsigned,
                                     const unsigned, const unsigned,
                                     const unsigned, const unsigned);

  const unsigned createRawTexture2D(const unsigned, const unsigned,
                                    const unsigned);
  const unsigned createOutputBuffer(const unsigned,
                                    const std::vector<unsigned>&);

  const unsigned* getSamplers() const;
  const bool createSamplers();

  std::vector<unsigned>* getUniformBuffers(const unsigned programId);
  std::vector<unsigned>* getUniformBuffers(const unsigned programId,
                                           const unsigned numberOfBuffers);

  // void createUniformBuffers(const unsigned programId,
  //                           const unsigned numberOfBuffers);

  const unsigned addMeshIndexCount(const std::pair<unsigned, unsigned>);
  std::pair<unsigned, unsigned> getIndexCount(const unsigned);

  // const unsigned updateInstanceBuffer(const std::vector<glm::mat4>&);
  // const unsigned createDepthBuffer(const unsigned);

  const glm::mat4 getIdentityMatrix() const;
private:
  // constructor
  OpenGLResourceManager();
};

} // namespace gl

} // namespace zge

#endif
