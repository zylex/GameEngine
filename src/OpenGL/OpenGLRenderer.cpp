#ifdef OPEN_GL

#include <iostream>
#include <GL/glew.h>

#include "IGame.h"
#include "DepthState.h"
#include "RasterState.h"
#include "AlphaState.h"

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

  this->enableAntiClockwiseCulling();

  return Renderer::initialise();
}

void OpenGLRenderer::setShaderProgram(const unsigned programId)
{
  glUseProgram(programId);

  // unsigned previousProgramId = this->getShaderProgramId();
  // unsigned attribNumber = 6;

  // OpenGLResourceManager* resourceManager =
  // OpenGLResourceManager::getInstance();
  // std::vector<ShaderInputElement> previousInputElements =
  //     resourceManager->getInputElements(previousProgramId).second;

  // for (int i = 0; i < previousInputElements.size(); ++i)
  // {
  //   glDisableVertexAttribArray(i + attribNumber);
  // }

  // std::pair<unsigned, std::vector<ShaderInputElement> > inputElements =
  //     resourceManager->getInputElements(programId);
  // std::vector<ShaderInputElement>::iterator end = inputElements.second.end();
  // unsigned startPointer = sizeof(glm::mat4);

  // glBindBuffer(GL_ARRAY_BUFFER, resourceManager->getInstanceBuffer());
  // glBindVertexBuffer(0, resourceManager->getInstanceBuffer(), 0,
  //                    inputElements.first);

  // instanceBuffer
  // glBindBuffer(GL_ARRAY_BUFFER, this->getInstanceBuffer());
  // for (unsigned i = 0; i < 4; ++i)
  // {
  // glEnableVertexAttribArray(2 + i);
  // glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) +
  // 16,
  //                       (const GLvoid*)(sizeof(float) * i * 4));
  // glVertexAttribDivisor(2 + i, 1);

  //   glVertexAttribFormat(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * i * 4);
  //   glVertexAttribBinding(2 + i, 0);
  //   glVertexBindingDivisor(2 + i, 1);
  // }

  // for (std::vector<ShaderInputElement>::iterator it =
  //          inputElements.second.begin();
  //      it IS_NOT end; ++it)
  // {
  // glEnableVertexAttribArray(attribNumber);
  // glVertexAttribPointer(attribNumber, it->numberOfElements,
  //                       (it->elementIsFloat ? GL_FLOAT : GL_INT), GL_FALSE,
  //                       inputElements.first, &startPointer);
  // glVertexAttribDivisor(attribNumber, 1);

  // glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(position,
  // Vertex));
  // glVertexAttribBinding(0, 0);
  // glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(normal, Vertex));
  // glVertexAttribBinding(1, 0);
  // glVertexAttribFormat(2, 4, GL_UNSIGNED_BYTE, GL_TRUE,
  //                      offsetof(color, Vertex));
  // glVertexAttribBinding(2, 0);

  //   glVertexAttribFormat(0, it->numberOfElements,
  //                        (it->elementIsFloat ? GL_FLOAT : GL_INT), GL_FALSE,
  //                        startPointer);
  //   glVertexAttribBinding(attribNumber, 0);
  //   glVertexBindingDivisor(attribNumber, 1);

  //   startPointer += it->numberOfElements *
  //                   (it->elementIsFloat ? sizeof(float) : sizeof(int));
  //   ++attribNumber;
  // }

  Renderer::setShaderProgram(programId);
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

  // glBindBuffer(GL_ARRAY_BUFFER, instanceBufferId);
  // for (unsigned i = 0; i < 4; ++i)
  // {
  //   glEnableVertexAttribArray(2 + i);
  //   glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE,
  //                         instanceDataUnitByteSize,
  //                         (const GLvoid*)(sizeof(float) * i * 4));
  //   glVertexAttribDivisor(2 + i, 1);
  // }

  // glBindVertexArray(meshIndexCount.first);
  // for (unsigned i = 0; i < 4; ++i)
  // {
  // glEnableVertexAttribArray(2 + i);
  // glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) +
  // 16,
  //                       (const GLvoid*)(sizeof(float) * i * 4));
  // glVertexAttribDivisor(2 + i, 1);

  //   glVertexAttribFormat(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * i * 4);
  //   glVertexAttribBinding(2 + i, 0);
  //   glVertexBindingDivisor(2 + i, 1);
  // }

  // unsigned attribNumber = 6;

  // std::pair<unsigned, std::vector<ShaderInputElement> > inputElements =
  //     resourceManager->getInputElements(this->getShaderProgramId());
  // std::vector<ShaderInputElement>::iterator end = inputElements.second.end();
  // unsigned startPointer = sizeof(glm::mat4);

  // glBindVertexBuffer(0, resourceManager->getInstanceBuffer(), 0,
  //                    inputElements.first);

  // for (std::vector<ShaderInputElement>::iterator it =
  //          inputElements.second.begin();
  //      it IS_NOT end; ++it)
  // {
  // glEnableVertexAttribArray(attribNumber);
  // glVertexAttribPointer(attribNumber, it->numberOfElements,
  //                       (it->elementIsFloat ? GL_FLOAT : GL_INT), GL_FALSE,
  //                       inputElements.first, &startPointer);
  // glVertexAttribDivisor(attribNumber, 1);

  // glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(position,
  // Vertex));
  // glVertexAttribBinding(0, 0);
  // glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(normal, Vertex));
  // glVertexAttribBinding(1, 0);
  // glVertexAttribFormat(2, 4, GL_UNSIGNED_BYTE, GL_TRUE,
  //                      offsetof(color, Vertex));
  // glVertexAttribBinding(2, 0);

  //   glVertexAttribFormat(0, it->numberOfElements,
  //                        (it->elementIsFloat ? GL_FLOAT : GL_INT), GL_FALSE,
  //                        startPointer);
  //   glVertexAttribBinding(attribNumber, 0);
  //   glVertexBindingDivisor(attribNumber, 1);

  //   startPointer += it->numberOfElements *
  //                   (it->elementIsFloat ? sizeof(float) : sizeof(int));
  //   ++attribNumber;
  // }
  // glBindVertexArray(GL_NONE);

  glBindVertexArray(meshIndexCount.first);
  glDrawElementsInstanced(GL_TRIANGLES, meshIndexCount.second, GL_UNSIGNED_INT,
                          nullptr, numberOfInstances);
  glBindVertexArray(GL_NONE);
}

void OpenGLRenderer::executeComputeShader()
{
  // TODO: implement executeComputeShader
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
  if (this->currentOutputId IS_NOT outputId)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, outputId);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->currentOutputId = outputId;
  }
}

void OpenGLRenderer::enableDepth()
{
  if (this->getDepthState() IS_NOT ZGE_DEPTH_ON)
  {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
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

void OpenGLRenderer::enableReadDepth()
{
  if (this->getDepthState() IS_NOT ZGE_DEPTH_READ)
  {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    this->setDepthState(ZGE_DEPTH_READ);
  }
}

void OpenGLRenderer::enableAlphaBlending()
{
  if (this->getAlphaState() IS_NOT ALPHA_ENABLED)
  {
    // https://www.opengl.org/wiki/Blending
    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    this->setAlphaState(ALPHA_ENABLED);
  }
}

void OpenGLRenderer::disableAlphaBlending()
{
  if (this->getAlphaState() IS_NOT ALPHA_OFF)
  {
    // FIXME
    glDisable(GL_BLEND);
    this->setAlphaState(ALPHA_OFF);
  }
}

void OpenGLRenderer::enableAntiClockwiseCulling()
{
  if (this->getRasterState() IS_NOT CULL_BACK)
  {
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    this->setRasterState(CULL_BACK);
  }
}

void OpenGLRenderer::disableCulling()
{
  if (this->getRasterState() IS_NOT CULL_OFF)
  {
    glDisable(GL_CULL_FACE);
    glFrontFace(GL_NONE);
    this->setRasterState(CULL_OFF);
  }
}

void OpenGLRenderer::setWindow(GLFWwindow* win)
{
  this->window = win;
  glfwMakeContextCurrent(this->window);
}

void OpenGLRenderer::swapBuffers() { glfwSwapBuffers(this->window); }

} // namespace gl

} // namespace zge

#endif
