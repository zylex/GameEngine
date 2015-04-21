#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "IResourceManager.h"

#include "MeshFactory.h"

namespace zge
{

class ResourceManager : public IResourceManager
{
private:
  MeshFactory meshFactory;

  unsigned instanceBufferId;

  std::unordered_map<std::string, unsigned> existingMeshes;
  std::unordered_map<std::string, unsigned> existingShaders;
  std::unordered_map<std::string, unsigned> existingShaderPrograms;
  // std::vector<unsigned> existingOutputs;
  // std::vector<unsigned> existingTextures;
  // std::vector<std::pair<unsigned, unsigned> > meshIndexCounts;
  //                 ProgId      list of bufferIds
  // std::unordered_map<unsigned, std::vector<unsigned> > uniformBuffers;
  //                            ProgId   ShaderType   list of bufferIds
  // std::unordered_map<std::pair<unsigned, unsigned>, std::vector<unsigned> >
  //     constantBuffers;

public:
  virtual ~ResourceManager() NOEXCEPT = default;

  const unsigned createMeshFromFile(std::string);

  const unsigned getSquareMesh();
  virtual const unsigned getInstanceBuffer();
  void setInstanceBuffer(const unsigned);

  const unsigned shaderExists(const void*, const std::size_t, const unsigned);
  void addShader(const void*, const std::size_t, const unsigned,
                 const unsigned);
  const unsigned shaderProgramExists(const unsigned, const unsigned,
                                     const unsigned, const unsigned,
                                     const unsigned, const unsigned);
  void addShaderProgram(const unsigned, const unsigned, const unsigned,
                        const unsigned, const unsigned, const unsigned,
                        const unsigned);

  std::unordered_map<std::string, unsigned> getExistingMeshes() const;
  std::unordered_map<std::string, unsigned> getExistingShaders() const;
  std::unordered_map<std::string, unsigned> getExistingShaderPrograms() const;
  // std::vector<unsigned> getExistingOutputs() const;
  // std::vector<unsigned> getExistingTextures() const;

  // const unsigned addMeshIndexCount(const std::pair<unsigned, unsigned>);
  // std::pair<unsigned, unsigned> getIndexCount(const unsigned);

  // std::vector<unsigned>* getUniformBuffers(const unsigned);
  // std::vector<unsigned>* getUniformBuffers(const unsigned, const unsigned);
  //void setUniformBuffers(const unsigned, std::vector<unsigned>);

private:
  const std::string getShaderKey(const void*, const std::size_t,
                                 const unsigned);
  const std::string getShaderProgramKey(const unsigned, const unsigned,
                                        const unsigned, const unsigned,
                                        const unsigned, const unsigned);
};

} // namespace zge

#endif
