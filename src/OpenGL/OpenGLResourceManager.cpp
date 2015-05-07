#ifdef OPEN_GL
#include <iostream>
#include <iterator>
#include <string>

#include <GL/glew.h>

#ifdef USE_ANT
#include "Stats.h"
#endif

#include "Preprocessors.h"

#include "IGame.h"
#include "ShaderType.h"
#include "SamplerType.h"
#include "TextureType.h"

#include "OpenGLResourceManager.h"

namespace zge
{

IResourceManager* IResourceManager::getInstance()
{
  return gl::OpenGLResourceManager::getInstance();
}

namespace gl
{

OpenGLResourceManager* OpenGLResourceManager::getInstance()
{
  static OpenGLResourceManager instance;
  return &instance;
}

OpenGLResourceManager::OpenGLResourceManager() : samplers{ 0, 0, 0, 0 }
{
  // constructor
}

OpenGLResourceManager::~OpenGLResourceManager() noexcept
{
  // destructor
  std::unordered_map<std::string, unsigned> existingMeshes =
      this->getExistingMeshes();
  for (std::unordered_map<std::string, unsigned>::iterator i =
           existingMeshes.begin();
       i != existingMeshes.end(); ++i)
  {
    glDeleteVertexArrays(1, &(i->second));
  }

  std::unordered_map<std::string, unsigned> existingShaders =
      this->getExistingShaders();
  for (std::unordered_map<std::string, unsigned>::iterator i =
           existingShaders.begin();
       i != existingShaders.end(); ++i)
  {
    glDeleteShader(i->second);
  }

  std::unordered_map<std::string, unsigned> existingShaderPrograms =
      this->getExistingShaderPrograms();
  for (std::unordered_map<std::string, unsigned>::iterator i =
           existingShaderPrograms.begin();
       i != existingShaderPrograms.end(); ++i)
  {
    glDeleteProgram(i->second);
  }

  glDeleteFramebuffers(this->existingOutputs.size(),
                       this->existingOutputs.data());

  glDeleteTextures(this->existingTextures.size(),
                   this->existingTextures.data());
}

OpenGLResourceManager::OpenGLResourceManager(const OpenGLResourceManager& other)
{
  // copy constructor
}

OpenGLResourceManager& OpenGLResourceManager::operator=(
    const OpenGLResourceManager& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

OpenGLResourceManager::OpenGLResourceManager(
    OpenGLResourceManager&& other) noexcept
{
  // move constructor (C++11)
}

OpenGLResourceManager& OpenGLResourceManager::operator=(
    OpenGLResourceManager&& other) noexcept
{
  // move assignment operator (C++11)
  return *(new OpenGLResourceManager(other));
}

const unsigned OpenGLResourceManager::createMesh(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec3>& normals,
    const std::vector<glm::uvec3>& indices)
{
  if (vertices.size() IS_NOT normals.size())
  {
    std::cerr
        << "Error creating mesh, not the same amount of vertices and normals"
        << std::endl;
    return 0;
  }
#ifdef USE_ANT
  Stats::numberOfVertices += vertices.size();
  Stats::numberOfTriangles += indices.size();
#endif

  unsigned IDs[4];
  // vertexBuffer
  glGenBuffers(3, IDs);
  glBindBuffer(GL_ARRAY_BUFFER, IDs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
               vertices.data(), GL_STATIC_DRAW);
  // normalBuffer
  glBindBuffer(GL_ARRAY_BUFFER, IDs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
               normals.data(), GL_STATIC_DRAW);

  // indexBuffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IDs[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * indices.size(),
               indices.data(), GL_STATIC_DRAW);

  // vertexArray
  glGenVertexArrays(1, &IDs[3]);
  glBindVertexArray(IDs[3]);
  // vertexBuffer
  glBindBuffer(GL_ARRAY_BUFFER, IDs[0]);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribDivisor(0, 0);
  // normalBuffer
  glBindBuffer(GL_ARRAY_BUFFER, IDs[1]);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, 0);
  glVertexAttribDivisor(1, 0);
  // indexBuffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IDs[2]);

  glBindBuffer(GL_ARRAY_BUFFER, getInstanceBuffer());
  // instanceBuffer
  for (unsigned i = 0; i < 4; ++i)
  {
    glEnableVertexAttribArray(2 + i);
    glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                          (const GLvoid*)(sizeof(float) * i * 4));
    glVertexAttribDivisor(2 + i, 1);
    // TODO: figure out a way to make instance modifiable platform agnostically
  }

  glBindVertexArray(GL_NONE);
  glDeleteBuffers(3, IDs);

  return this->addMeshIndexCount({ IDs[3], indices.size() * 3 });
}

const unsigned OpenGLResourceManager::createMesh(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec3>& normals,
    const std::vector<glm::uvec3>& indices,
    const std::vector<glm::vec2>& textureCoordinates,
    const std::vector<glm::vec3>& tangents,
    const std::vector<glm::vec3>& bitangents)
{
  return 0;
}

// const unsigned OpenGLResourceManager::createDepthBuffer(
//    const unsigned textureId)
//{
//  unsigned result;
//  glGenFramebuffers(1, &result);
//
//  glBindTexture(GL_TEXTURE_2D, textureId);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCREEN_WIDTH(),
//               SCREEN_HEIGHT(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
//                  GL_NEAREST_MIPMAP_NEAREST);
//  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
//                  GL_NEAREST_MIPMAP_NEAREST);
//  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//
//  glBindFramebuffer(GL_FRAMEBUFFER, result);
//  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
//                         textureId, 0);
//
//  // Disable writes to the color buffer
//  glDrawBuffer(GL_NONE);
//  glReadBuffer(GL_NONE);
//
//  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//
//  if (status != GL_FRAMEBUFFER_COMPLETE)
//  {
//    std::cerr << "OpenGL FrameBuffer error, status: 0x" << status <<
//    std::endl;
//    return 0;
//  }
//
//  return result;
//}

const unsigned OpenGLResourceManager::getInstanceBuffer()
{
  unsigned instanceBufferId = ResourceManager::getInstanceBuffer();
  if (not instanceBufferId)
  {
    glGenBuffers(1, &instanceBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, instanceBufferId);
    glm::mat4 dummyData[1];
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), dummyData,
                 GL_DYNAMIC_DRAW);
    ResourceManager::setInstanceBuffer(instanceBufferId);
  }

  return instanceBufferId;
}

// const unsigned OpenGLResourceManager::updateInstanceBuffer(
//    const std::vector<glm::mat4>& instanceData)
//{
//  if (not this->instanceBufferId) // make sure it exists
//  {
//    getInstanceBuffer();
//  }
//  glBindBuffer(GL_ARRAY_BUFFER, this->instanceBufferId);
//  glBufferData(GL_ARRAY_BUFFER, instanceData.size() * sizeof(glm::mat4),
//               instanceData.data(), GL_DYNAMIC_DRAW);
//
//  return this->instanceBufferId;
//}

const unsigned OpenGLResourceManager::compileShaderCode(
    const void* shaderCode, const size_t shaderCodeSize,
    const unsigned shaderType)
{
  // returns 0 if not exists
  unsigned shaderId =
      this->shaderExists(shaderCode, shaderCodeSize, shaderType);
  if (shaderCode IS_NOT nullptr and shaderId IS 0)
  {
    switch (shaderType)
    {
      case COMPUTE_SHADER:
      {
        shaderId = glCreateShader(GL_COMPUTE_SHADER);
        break;
      }
      case VERTEX_SHADER:
      {
        shaderId = glCreateShader(GL_VERTEX_SHADER);
        break;
      }
      case GEOMETRY_SHADER:
      {
        shaderId = glCreateShader(GL_GEOMETRY_SHADER);
        break;
      }
      case HULL_SHADER:
      {
        shaderId = glCreateShader(GL_TESS_CONTROL_SHADER);
        break;
      }
      case DOMAIN_SHADER:
      {
        shaderId = glCreateShader(GL_TESS_EVALUATION_SHADER);
        break;
      }
      case PIXEL_SHADER:
      {
        shaderId = glCreateShader(GL_FRAGMENT_SHADER);
        break;
      }
      default:
      {
        return 0;
      }
    }

    glShaderSource(shaderId, 1, (const char**)&shaderCode, NULL);
    glCompileShader(shaderId);

    // check for errors
    int infoLogLength, compilationSuccess = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilationSuccess);
    if (not compilationSuccess)
    {
      glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
      if (infoLogLength > 0)
      {
        std::vector<char> shaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(shaderId, infoLogLength, NULL,
                           &shaderErrorMessage[0]);
        std::cerr << &shaderErrorMessage[0] << std::endl;
      }
      return 0;
    }

    this->addShader(shaderCode, shaderCodeSize, shaderType, shaderId);
  }

  return shaderId;
}

const unsigned OpenGLResourceManager::createShaderProgram(
    const unsigned computeShaderId, const unsigned vertexShaderId,
    const unsigned geometryShaderId, const unsigned hullShaderId,
    const unsigned domainShaderId, const unsigned pixelShaderId)
{
  // returns 0 if not exists
  unsigned programId = this->shaderProgramExists(
      computeShaderId, vertexShaderId, geometryShaderId, hullShaderId,
      domainShaderId, pixelShaderId);
  if (programId IS 0 and (computeShaderId IS_NOT 0 or vertexShaderId IS_NOT 0 or
                          geometryShaderId IS_NOT 0 or hullShaderId IS_NOT 0 or
                          domainShaderId IS_NOT 0 or pixelShaderId IS_NOT 0))
  {
    programId = glCreateProgram();
    if (computeShaderId IS_NOT 0)
    {
      glAttachShader(programId, computeShaderId);
    }
    if (vertexShaderId IS_NOT 0)
    {
      glAttachShader(programId, vertexShaderId);
    }
    if (geometryShaderId IS_NOT 0)
    {
      glAttachShader(programId, geometryShaderId);
    }
    if (hullShaderId IS_NOT 0)
    {
      glAttachShader(programId, hullShaderId);
    }
    if (domainShaderId IS_NOT 0)
    {
      glAttachShader(programId, domainShaderId);
    }
    if (pixelShaderId IS_NOT 0)
    {
      glAttachShader(programId, pixelShaderId);
    }
    glLinkProgram(programId);

    // check for errors
    int result = GL_FALSE;

    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    if (not result)
    {
      int infoLogLength;
      glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
      if (infoLogLength > 0)
      {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programId, infoLogLength, NULL,
                            &programErrorMessage[0]);
        std::cerr << &programErrorMessage[0] << std::endl;
        return 0;
      }
    }
    this->addShaderProgram(computeShaderId, vertexShaderId, geometryShaderId,
                           hullShaderId, domainShaderId, pixelShaderId,
                           programId);
  }

  return programId;
}

const unsigned OpenGLResourceManager::createRawTexture2D(
    const unsigned width, const unsigned height, const unsigned textureType)
{
  unsigned result;
  glGenTextures(1, &result);
  glBindTexture(GL_TEXTURE_2D, result);

  switch (textureType)
  {
    case ZGE_DEPTH:
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                   GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
      break;
    }
    case ZGE_COLOR:
    {
      glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_FLOAT,
                   NULL);
      break;
    }
    default:
    {
      return 0;
    }
  }

  this->existingTextures.push_back(result);

  return result;
}

const unsigned OpenGLResourceManager::createOutputBuffer(
    const unsigned depthTexture, const std::vector<unsigned>& outputTextures)
{
  unsigned result;
  glGenFramebuffers(1, &result);
  glBindFramebuffer(GL_FRAMEBUFFER, result);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depthTexture, 0);
  if (outputTextures.size() IS_NOT 0)
  {
    for (int i = 0; i < outputTextures.size(); ++i)
    {
      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                           outputTextures[i], 0);
    }
  }
  else
  {
    // Disable read/writes to the color buffer
    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_NONE);
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) IS_NOT GL_FRAMEBUFFER_COMPLETE)
  {
    return 0;
  }

  this->existingOutputs.push_back(result);

  return result;
}

const unsigned* OpenGLResourceManager::getSamplers() const
{
  return this->samplers;
}

const bool OpenGLResourceManager::createSamplers()
{
  glGenSamplers(TOTAL_SAMPLERS, this->samplers);

  // POINT_WRAP
  glSamplerParameterf(this->samplers[POINT_WRAP], GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST_MIPMAP_NEAREST);
  glSamplerParameterf(this->samplers[POINT_WRAP], GL_TEXTURE_MAG_FILTER,
                      GL_NEAREST_MIPMAP_NEAREST);
  glSamplerParameterf(this->samplers[POINT_WRAP], GL_TEXTURE_WRAP_S, GL_REPEAT);
  glSamplerParameterf(this->samplers[POINT_WRAP], GL_TEXTURE_WRAP_T, GL_REPEAT);

  // POINT_CLAMP
  glSamplerParameterf(this->samplers[POINT_CLAMP], GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST_MIPMAP_NEAREST);
  glSamplerParameterf(this->samplers[POINT_CLAMP], GL_TEXTURE_MAG_FILTER,
                      GL_NEAREST_MIPMAP_NEAREST);
  glSamplerParameterf(this->samplers[POINT_CLAMP], GL_TEXTURE_WRAP_S,
                      GL_CLAMP_TO_EDGE);
  glSamplerParameterf(this->samplers[POINT_CLAMP], GL_TEXTURE_WRAP_T,
                      GL_CLAMP_TO_EDGE);

  // LINEAR_WRAP
  glSamplerParameterf(this->samplers[LINEAR_WRAP], GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
  glSamplerParameterf(this->samplers[LINEAR_WRAP], GL_TEXTURE_MAG_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
  glSamplerParameterf(this->samplers[LINEAR_WRAP], GL_TEXTURE_WRAP_S,
                      GL_REPEAT);
  glSamplerParameterf(this->samplers[LINEAR_WRAP], GL_TEXTURE_WRAP_T,
                      GL_REPEAT);

  // LINEAR_CLAMP
  glSamplerParameterf(this->samplers[LINEAR_CLAMP], GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
  glSamplerParameterf(this->samplers[LINEAR_CLAMP], GL_TEXTURE_MAG_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
  glSamplerParameterf(this->samplers[LINEAR_CLAMP], GL_TEXTURE_WRAP_S,
                      GL_CLAMP_TO_EDGE);
  glSamplerParameterf(this->samplers[LINEAR_CLAMP], GL_TEXTURE_WRAP_T,
                      GL_CLAMP_TO_EDGE);
  return true;
}

std::vector<unsigned>* OpenGLResourceManager::getUniformBuffers(
    const unsigned programId)
{
  return &this->uniformBuffers[programId];
}

std::vector<unsigned>* OpenGLResourceManager::getUniformBuffers(
    const unsigned programId, const unsigned numberOfBuffers)
{
  std::vector<unsigned>* uniformBuffersArray =
      this->getUniformBuffers(programId);

  unsigned numberOfBuffersToCreate =
      numberOfBuffers - uniformBuffersArray->size();
  unsigned newBuffers[numberOfBuffersToCreate];
  glGenBuffers(numberOfBuffersToCreate, newBuffers);

  uniformBuffersArray->insert(uniformBuffersArray->end(), newBuffers,
                              newBuffers + numberOfBuffersToCreate);

  return uniformBuffersArray;
}

const unsigned OpenGLResourceManager::addMeshIndexCount(
    const std::pair<unsigned, unsigned> meshIndexCount)
{
  this->meshIndexCounts.push_back(meshIndexCount);
  return this->meshIndexCounts.size() - 1;
}

std::pair<unsigned, unsigned> OpenGLResourceManager::getIndexCount(
    const unsigned meshId)
{
  return this->meshIndexCounts[meshId];
}

const bool OpenGLResourceManager::initialise()
{
  // TODO: implement initialise?
  this->setIdentityMatrix(glm::mat4());
  return true;
}

} // namespace gl

} // namespace zge

#endif
