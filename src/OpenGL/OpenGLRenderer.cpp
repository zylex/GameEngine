#ifdef OPEN_GL

#include <iostream>
#include <GL/glew.h>

#include "IGame.h"
#include "DepthState.h"

#include "OpenGLResourceManager.h"

#include "OpenGLRenderer.h"

namespace zge
{

IRenderer* IRenderer::getInstance()
{
  return gl::OpenGLRenderer::getInstance();
}

namespace gl
{

OpenGLRenderer* OpenGLRenderer::getInstance()
{
  static OpenGLRenderer instance;
  return &instance;
}

OpenGLRenderer::OpenGLRenderer()
{
  // constructor
}

OpenGLRenderer::~OpenGLRenderer() noexcept
{
  // destructor
}

OpenGLRenderer::OpenGLRenderer(const OpenGLRenderer& other)
{
  // copy constructor
}

OpenGLRenderer& OpenGLRenderer::operator=(const OpenGLRenderer& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

OpenGLRenderer::OpenGLRenderer(OpenGLRenderer&& other) noexcept
{
  // move constructor (C++11)
}

OpenGLRenderer& OpenGLRenderer::operator=(OpenGLRenderer&& other) noexcept
{
  // move assignment operator (C++11)
  return *(new OpenGLRenderer(other));
}

const bool OpenGLRenderer::initialise()
{
  glClearColor(zge::CLEAR_COLOR.x, zge::CLEAR_COLOR.y, zge::CLEAR_COLOR.z,
               zge::CLEAR_COLOR.w);

  this->window = glfwGetCurrentContext();
  return Renderer::initialise();
}

void OpenGLRenderer::setShaderProgram(const unsigned programId)
{
  glUseProgram(programId);
  Renderer::setShaderProgram(programId);
}

void OpenGLRenderer::setConstants(
    const void** bufferDataArray,
    const std::vector<unsigned long> bufferDataByteSizes,
    const unsigned shaderType)
{
  unsigned numberOfBuffers = bufferDataByteSizes.size();
  OpenGLResourceManager* resourceManager = OpenGLResourceManager::getInstance();
  std::vector<unsigned>* buffers = resourceManager->getUniformBuffers(
      this->getShaderProgramId(), numberOfBuffers);
  for (int i = 0; i < numberOfBuffers; ++i)
  {
    // update the buffer
    glBindBuffer(GL_UNIFORM_BUFFER, (*buffers)[i]);
    glBufferData(GL_UNIFORM_BUFFER, bufferDataByteSizes[i], bufferDataArray[i],
                 GL_DYNAMIC_DRAW);
    // bind the buffer
    glBindBufferBase(GL_UNIFORM_BUFFER, i, (*buffers)[i]);
  }
}

void OpenGLRenderer::setConstant(const unsigned indexPosition,
                                 const void* bufferData,
                                 const unsigned long bufferDataByteSize,
                                 const unsigned shaderType)
{
  OpenGLResourceManager* resourceManager = OpenGLResourceManager::getInstance();
  const unsigned programId = this->getShaderProgramId();
  std::vector<unsigned>* buffers =
      resourceManager->getUniformBuffers(programId);
  // update the buffer
  if (buffers->size() < indexPosition + 1)
  {
    buffers = resourceManager->getUniformBuffers(programId, indexPosition + 1);
  }
  glBindBuffer(GL_UNIFORM_BUFFER, (*buffers)[indexPosition]);
  glBufferData(GL_UNIFORM_BUFFER, bufferDataByteSize, bufferData,
               GL_DYNAMIC_DRAW);
  // bind the buffer
  glBindBufferBase(GL_UNIFORM_BUFFER, indexPosition, (*buffers)[indexPosition]);
}

void OpenGLRenderer::executeInstancedShader(
    const unsigned meshId, const void* instanceDataArray,
    const unsigned long instanceDataUnitByteSize,
    const unsigned numberOfInstances)
{
  OpenGLResourceManager* resourceManager = OpenGLResourceManager::getInstance();
  std::pair<unsigned, unsigned> meshIndexCount =
      resourceManager->getIndexCount(meshId);

  unsigned instanceBufferId = resourceManager->getInstanceBuffer();

  // bind instance buffer
  glBindBuffer(GL_ARRAY_BUFFER, instanceBufferId);
  // update instance buffer
  glBufferData(GL_ARRAY_BUFFER, instanceDataUnitByteSize * numberOfInstances,
               instanceDataArray, GL_DYNAMIC_DRAW);
  glBindVertexArray(meshIndexCount.first);
  glDrawElementsInstanced(GL_TRIANGLES, meshIndexCount.second, GL_UNSIGNED_INT,
                          nullptr, numberOfInstances);
  glBindVertexArray(GL_NONE);
}

void OpenGLRenderer::executeComputeShader()
{
  // TODO: implement executeComputeShader
}

void OpenGLRenderer::OpenGLRenderer::executeShader()
{
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
}

void OpenGLRenderer::OpenGLRenderer::setTextures(
    const std::vector<unsigned> textureIds,
    const std::vector<unsigned> samplerTypes, const unsigned shaderType)
{
  unsigned count = textureIds.size();
  if (count IS_NOT samplerTypes.size())
  {
    std::cerr << "ERROR: OpenGLRenderer::setTextures: Number of textures does "
                 "not match number of samplers." << std::endl;
    return;
  }

  OpenGLResourceManager* resourceManager = OpenGLResourceManager::getInstance();
  for (int i = 0; i < count; ++i)
  {
    glBindSampler(textureIds[i],
                  resourceManager->getSamplers()[samplerTypes[i]]);
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, textureIds[i]);
  }
}

void OpenGLRenderer::setTexture(const unsigned textureLocation,
                                const unsigned textureId,
                                const unsigned samplerType,
                                const unsigned shaderType)
{
  OpenGLResourceManager* resourceManager = OpenGLResourceManager::getInstance();
  glBindSampler(textureId, resourceManager->getSamplers()[samplerType]);
  glActiveTexture(GL_TEXTURE0 + textureLocation);
  glBindTexture(GL_TEXTURE_2D, textureId);
}

void OpenGLRenderer::setOutput(const unsigned outputId)
{
  glBindFramebuffer(GL_FRAMEBUFFER, outputId);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::enableDepth()
{
  if (this->getDepthState() IS_NOT ZGE_DEPTH_ON)
  {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    this->setDepthState(ZGE_DEPTH_ON);
  }
}

void OpenGLRenderer::disableDepth()
{
  if (this->getDepthState() IS_NOT ZGE_DEPTH_OFF)
  {
    glDisable(GL_DEPTH_TEST);
    this->setDepthState(ZGE_DEPTH_OFF);
  }
}

void OpenGLRenderer::enableAntiClockwiseCulling()
{
  glFrontFace(GL_CW);
  glCullFace(GL_BACK);
}

void OpenGLRenderer::disableCulling() { glFrontFace(GL_NONE); }

void OpenGLRenderer::setWindow(GLFWwindow* win) { this->window = win; }

void OpenGLRenderer::swapBuffers() { glfwSwapBuffers(this->window); }

} // namespace gl

} // namespace zge

#endif
