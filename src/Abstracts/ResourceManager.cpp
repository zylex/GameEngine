#include <sstream>
#include <iostream>

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
  if (shaderCode IS_NOT nullptr)
  {
    std::string key = getShaderKey(shaderCode, shaderCodeSize, shaderType);

    std::unordered_map<std::string, unsigned>::iterator i =
        this->existingShaders.find(key);
    if (i IS_NOT this->existingShaders.end())
    {
      return i->second;
    }
  }
  else
  {
    std::cerr << "Error Shader code is nullptr." << std::endl;
  }
  return 0;
}

void ResourceManager::addShader(const void* shaderCode,
                                const std::size_t shaderCodeSize,
                                const unsigned shaderType,
                                const unsigned shaderId)
{
  if (shaderCode IS_NOT nullptr)
  {
    std::string key = getShaderKey(shaderCode, shaderCodeSize, shaderType);

    this->existingShaders[key] = shaderId;
  }
}

const unsigned ResourceManager::shaderProgramExists(
    const unsigned computeShaderId, const unsigned vertexShaderId,
    const unsigned geometryShaderId, const unsigned hullShaderId,
    const unsigned domainShaderId, const unsigned pixelShaderId)
{
  if (computeShaderId IS_NOT 0 and vertexShaderId IS_NOT 0 and
      geometryShaderId IS_NOT 0 and hullShaderId IS_NOT 0 and
      domainShaderId IS_NOT 0 and pixelShaderId IS_NOT 0)
  {
    std::string key =
        getShaderProgramKey(computeShaderId, vertexShaderId, geometryShaderId,
                            hullShaderId, domainShaderId, pixelShaderId);

    std::unordered_map<std::string, unsigned>::iterator i =
        this->existingShaderPrograms.find(key);
    if (i IS_NOT this->existingShaderPrograms.end())
    {
      return i->second;
    }
  }
  return 0;
}

void ResourceManager::addShaderProgram(const unsigned computeShaderId,
                                       const unsigned vertexShaderId,
                                       const unsigned geometryShaderId,
                                       const unsigned hullShaderId,
                                       const unsigned domainShaderId,
                                       const unsigned pixelShaderId,
                                       const unsigned programId)
{
  if (programId IS_NOT 0 and computeShaderId IS_NOT 0 and
      vertexShaderId IS_NOT 0 and geometryShaderId IS_NOT 0 and
      hullShaderId IS_NOT 0 and domainShaderId IS_NOT 0 and
      pixelShaderId IS_NOT 0)
  {
    std::string key =
        getShaderProgramKey(computeShaderId, vertexShaderId, geometryShaderId,
                            hullShaderId, domainShaderId, pixelShaderId);

    this->existingShaderPrograms[key] = programId;
  }
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

// std::vector<unsigned> ResourceManager::getExistingTextures() const {
//  return this->existingTextures;
//}
//
// std::vector<unsigned> ResourceManager::getExistingOutputs() const {
//  return this->existingOutputs;
//}

// const unsigned ResourceManager::addMeshIndexCount(
//    const std::pair<unsigned, unsigned> meshIndexCount)
//{
//  this->meshIndexCounts.push_back(meshIndexCount);
//  return this->meshIndexCounts.size() - 1;
//}
//
// std::pair<unsigned, unsigned> ResourceManager::getIndexCount(
//    const unsigned meshId)
//{
//  return this->meshIndexCounts[meshId];
//}

const unsigned ResourceManager::getInstanceBuffer()
{
  return this->instanceBufferId;
}

void ResourceManager::setInstanceBuffer(const unsigned instanceBufferId)
{
  this->instanceBufferId = instanceBufferId;
}

// std::vector<unsigned>* ResourceManager::getUniformBuffers(
//     const unsigned programId)
// {
//   return &this->uniformBuffers[programId];
// }

// std::vector<unsigned>* ResourceManager::getUniformBuffers(
//     const unsigned programId, const unsigned shaderType)
// {
//   return &this->constantBuffers[{ programId, shaderType }];
// }

//void ResourceManager::setUniformBuffers(const unsigned programId,
//                                        std::vector<unsigned> uniformBuffers)
//{
//  this->uniformBuffers[programId] = uniformBuffers;
//}

} // namespace zge
