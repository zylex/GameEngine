#include <sstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Preprocessors.h"

#include "IGame.h"

#include "ResourceManager.h"

namespace zge
{

ResourceManager::ResourceManager()
{
  // constructor
}

const bool ResourceManager::initialise()
{
  this->perspectiveMatrix =
      this->getIdentityMatrix() *
      glm::perspective(45.0f, (float)SCREEN_WIDTH() / (float)SCREEN_HEIGHT(),
                       SCREEN_NEAR, SCREEN_DEPTH);
  this->orthographicMatrix =
      this->getIdentityMatrix() * glm::ortho(0.0f, (float)SCREEN_WIDTH(),
                                             (float)SCREEN_HEIGHT(), 0.0f,
                                             SCREEN_NEAR, SCREEN_DEPTH);
  // glm::ortho(T left, T right, T bottom, T top, T zNear, T zFar)
  return true;
}

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
  unsigned result = this->createMesh(this->meshFactory.getVertices(),
                                     this->meshFactory.getTextureCoordinates(),
                                     this->meshFactory.getNormals(),
                                     this->meshFactory.getIndices());
  if (result)
  {
    this->existingMeshes[key] = result;
  }
  return result;
}

const unsigned ResourceManager::getSquareMeshWithHole()
{
  std::string key = "::square_with_hole::";
  std::unordered_map<std::string, unsigned>::iterator i =
      this->existingMeshes.find(key);
  if (i IS_NOT this->existingMeshes.end())
  {
    return i->second;
  }

  this->meshFactory.generateSquareMeshWithHole();
  unsigned result = this->createMesh(this->meshFactory.getVertices(),
                                     this->meshFactory.getTextureCoordinates(),
                                     this->meshFactory.getNormals(),
                                     this->meshFactory.getIndices());
  if (result)
  {
    this->existingMeshes[key] = result;
  }
  return result;
}

const unsigned ResourceManager::getIcosphereMesh(const unsigned recursionLevel)
{
  std::ostringstream keyStream;
  keyStream << "::icosphere::" << recursionLevel << "::";
  std::unordered_map<std::string, unsigned>::iterator i =
      this->existingMeshes.find(keyStream.str());
  if (i IS_NOT this->existingMeshes.end())
  {
    return i->second;
  }

  this->meshFactory.generateIcosphereMesh(recursionLevel);
  unsigned result = this->createMesh(this->meshFactory.getVertices(),
                                     this->meshFactory.getTextureCoordinates(),
                                     this->meshFactory.getNormals(),
                                     this->meshFactory.getIndices());
  if (result)
  {
    this->existingMeshes[keyStream.str()] = result;
  }
  return result;
}

const unsigned ResourceManager::getBowlMesh(const float radius,
                                            const float height)
{
  std::string key = "::bowl::";
  std::unordered_map<std::string, unsigned>::iterator i =
      this->existingMeshes.find(key);
  if (i IS_NOT this->existingMeshes.end())
  {
    return i->second;
  }

  this->meshFactory.generateBowlMesh(radius, height);
  unsigned result = this->createMesh(this->meshFactory.getVertices(),
                                     this->meshFactory.getTextureCoordinates(),
                                     this->meshFactory.getNormals(),
                                     this->meshFactory.getIndices());
  if (result)
  {
    this->existingMeshes[key] = result;
  }
  return result;
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

const unsigned ResourceManager::getInstanceBuffer()
{
  return this->instanceBufferId;
}

void ResourceManager::setInstanceBuffer(const unsigned instanceBufferId)
{
  this->instanceBufferId = instanceBufferId;
}

const unsigned ResourceManager::textureExists(const std::string key)
{
  return this->existingTextures[key];
}

void ResourceManager::addTexture(const std::string key,
                                 const unsigned textureId)
{
  this->existingTextures[key] = textureId;
}

std::unordered_map<std::string, unsigned> ResourceManager::getExistingTextures()
    const
{
  return this->getExistingTextures();
}

void ResourceManager::setIdentityMatrix(glm::mat4 identityMatrix)
{
  this->identityMatrix = identityMatrix;
}

const glm::mat4& ResourceManager::getIdentityMatrix() const
{
  return this->identityMatrix;
}

const glm::mat4& ResourceManager::getPerspectiveMatrix() const
{
  return this->perspectiveMatrix;
}

const glm::mat4& ResourceManager::getOrthographicMatrix() const
{
  return this->orthographicMatrix;
}

} // namespace zge
