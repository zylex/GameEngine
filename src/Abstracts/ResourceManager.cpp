#include <sstream>

#include "Preprocessors.h"

#include "ResourceManager.h"

namespace zge
{

const unsigned ResourceManager::getSquareMesh()
{
  std::string key = "::square::";
  std::unordered_map<std::string, unsigned>::iterator i =
      this->existingMeshes.find(key);
  if (i IS_NOT this->existingMeshes.end())
  {
    return i->second;
  }

  this->meshFactory.generateSquareMesh();
  return createMesh(this->meshFactory.getVertices(),
                    this->meshFactory.getNormals(),
                    this->meshFactory.getIndices());
}

const unsigned ResourceManager::createMeshFromFile(std::string filepath)
{
  std::unordered_map<std::string, unsigned>::iterator i =
      this->existingMeshes.find(filepath);
  if (i IS_NOT this->existingMeshes.end())
  {
    return i->second;
  }

  // TODO: implement, proxy to MeshFactory possibly with assimp
  return 0;
}

const unsigned ResourceManager::shaderExists(const void* shaderCode,
                                             const std::size_t shaderCodeSize,
                                             const unsigned shaderType)
{
  std::ostringstream keyStream;
  keyStream << reinterpret_cast<std::size_t>(shaderCode)
            << "::" << shaderCodeSize << "::" << shaderType;
  std::string key = keyStream.str();

  return this->existingShaders[key];
}

void ResourceManager::addShader(const void* shaderCode,
                                const std::size_t shaderCodeSize,
                                const unsigned shaderType,
                                const unsigned shaderId)
{
  std::string key = getShaderKey(shaderCode, shaderCodeSize, shaderType);

  this->existingShaders.insert({ key, shaderId });
}

const unsigned ResourceManager::shaderProgramExists(
    const unsigned computerShaderId, const unsigned vertexShaderId,
    const unsigned geometryShaderId, const unsigned hullShaderId,
    const unsigned domainShaderId, const unsigned pixelShaderId)
{
  std::string key =
      getShaderProgramKey(computerShaderId, vertexShaderId, geometryShaderId,
                          hullShaderId, domainShaderId, pixelShaderId);

  std::unordered_map<std::string, unsigned>::iterator i =
      this->existingShaderPrograms.find(key);
  if (i IS_NOT this->existingShaderPrograms.end())
  {
    return i->second;
  }
  else
  {
    return 0;
  }
}

void ResourceManager::addShaderProgram(const unsigned computerShaderId,
                                       const unsigned vertexShaderId,
                                       const unsigned geometryShaderId,
                                       const unsigned hullShaderId,
                                       const unsigned domainShaderId,
                                       const unsigned pixelShaderId,
                                       const unsigned programId)
{
  std::string key =
      getShaderProgramKey(computerShaderId, vertexShaderId, geometryShaderId,
                          hullShaderId, domainShaderId, pixelShaderId);

  this->existingShaderPrograms[key] = programId;
}

const std::string ResourceManager::getShaderKey(
    const void* shaderCode, const std::size_t shaderCodeSize,
    const unsigned shaderType)
{
  std::ostringstream keyStream;
  keyStream << reinterpret_cast<std::size_t>(shaderCode)
            << "::" << shaderCodeSize << "::" << shaderType;
  return keyStream.str();
}

const std::string ResourceManager::getShaderProgramKey(
    const unsigned computerShaderId, const unsigned vertexShaderId,
    const unsigned geometryShaderId, const unsigned hullShaderId,
    const unsigned domainShaderId, const unsigned pixelShaderId)
{
  std::ostringstream keyStream;
  keyStream << computerShaderId << "::" << vertexShaderId
            << "::" << geometryShaderId << "::" << hullShaderId
            << "::" << domainShaderId << "::" << pixelShaderId;
  return keyStream.str();
}

std::unordered_map<std::string, unsigned> ResourceManager::getExistingMeshes()
    const
{
  return this->existingMeshes;
}

std::unordered_map<std::string, unsigned> ResourceManager::getExistingShaders()
    const
{
  return this->existingShaders;
}

std::unordered_map<std::string, unsigned>
ResourceManager::getExistingShaderPrograms() const
{
  return this->existingShaderPrograms;
}

std::vector<unsigned> ResourceManager::getExistingTextures() const
{
  return this->existingTextures;
}

std::vector<unsigned> ResourceManager::getExistingOutputs() const
{
  return this->existingOutputs;
}

const unsigned ResourceManager::addMeshIndexCount(
    const std::pair<unsigned, unsigned> meshIndexCount)
{
  this->meshIndexCounts.push_back(meshIndexCount);
  return this->meshIndexCounts.size() - 1;
}

std::pair<unsigned, unsigned> ResourceManager::getIndexCount(
    const unsigned meshId)
{
  return this->meshIndexCounts[meshId];
}

} // namespace zge
