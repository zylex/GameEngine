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
  unsigned instanceBufferId;

  unsigned samplers[TOTAL_SAMPLERS];
  // unordered_map<std::string, unsigned> texturesWithSamplers;
  //                 programId list of bufferIds
  std::unordered_map<unsigned, std::vector<unsigned> > uniformBuffers;

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

  const unsigned createMesh(const std::vector<glm::vec3>&,   // vertices
                            const std::vector<glm::vec3>&,   // normals
                            const std::vector<glm::uvec3>&); // indices

  const unsigned createDepthBuffer(const unsigned);

  const unsigned getInstanceBuffer();
  const unsigned updateInstanceBuffer(const std::vector<glm::mat4>&);

  const unsigned compileShaderCode(const void*, const std::size_t,
                                   const unsigned);
  const unsigned createShaderProgram(const unsigned, const unsigned,
                                     const unsigned, const unsigned,
                                     const unsigned, const unsigned);

  const unsigned createRawTexture2D(const unsigned, const unsigned,
                                    const unsigned);
  const unsigned createOutputBuffer(const unsigned,
                                    const std::vector<unsigned>);
  const unsigned* getSamplers() const;

  // const unsigned getTextureWithSampler(const unsigned textureId,
  //                                      const unsigned samplerId);

  const bool createSamplers();

  const std::vector<unsigned> getUniformBuffers(const unsigned programId);
  const std::vector<unsigned> getUniformBuffers(const unsigned programId,
                                                const unsigned numberOfBuffers);

  void createUniformBuffers(const unsigned programId,
                            const unsigned numberOfBuffers);

private:
  // constructor
  OpenGLResourceManager();

  // const std::string getTextureWithSamplerKey(const unsigned textureId,
  // const unsigned samplerId);
};

} // namespace gl

} // namespace zge

#endif