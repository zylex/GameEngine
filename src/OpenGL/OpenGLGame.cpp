#ifdef OPEN_GL
#include <iostream>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "OpenGLGame.h"
#include "IGame.h"
#include "OpenGLShader.h"
#include "InterpolatedVS.h"
#include "InterpolatedDepthPS.h"
// #include "InterpolatedPS.h"

namespace zge
{
namespace gl
{
using namespace glm;
using namespace std;

bool OpenGLGame::showDepth = false;

OpenGLGame::OpenGLGame(std::string applicationName)
{
  m_applicationName = applicationName;
  // constructor
}

OpenGLGame::~OpenGLGame() noexcept
{

  // destructor
}

OpenGLGame::OpenGLGame(const OpenGLGame& other)
{

  // copy constructor
}

OpenGLGame& OpenGLGame::operator=(const OpenGLGame& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

OpenGLGame::OpenGLGame(OpenGLGame&& other) noexcept
{

  // move constructor (C++11)
}

OpenGLGame& OpenGLGame::operator=(OpenGLGame&& other) noexcept
{
  // move assignment operator (C++11)
  return *(new OpenGLGame(other));
}

const int OpenGLGame::run()
{
  glfwSetErrorCallback(OpenGLGame::error_callback);

  if (!glfwInit())
  {
    cerr << "GLFW failed to initialise." << endl;
  }

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (!FULL_SCREEN)
  {
    SCREEN_WIDTH(WINDOWED_WIDTH);
    SCREEN_HEIGHT(WINDOWED_HEIGHT);
  }

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH(), SCREEN_HEIGHT(),
                                        m_applicationName.c_str(), NULL, NULL);

  if (!window)
  {
    cerr << "GLFW failed to initialise window." << endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  glewExperimental = true;
  if (glewInit() != GLEW_OK)
  {
    cerr << "GLEW failed to initialise." << endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwSwapInterval(1);
  glfwSetKeyCallback(window, OpenGLGame::key_callback);

  // glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glEnable(GL_CULL_FACE);
  // glFrontFace(GL_CW);
  // glCullFace(GL_BACK);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  vector<vec3> vertices;
  vertices.push_back(vec3(-1.0f, -1.0f, 0.0f));
  vertices.push_back(vec3(-1.0f, 1.0f, 0.0f));
  vertices.push_back(vec3(1.0f, 1.0f, 0.0f));
  vertices.push_back(vec3(1.0f, -1.0f, 0.0f));

  const uint shadowShader = compileShader(InterpolatedVS);
  glUseProgram(shadowShader);
  m_worldMatrixShadowLocation =
      glGetUniformLocation(shadowShader, "worldMatrix");
  assert(m_worldMatrixShadowLocation != 0xFFFFFFFF);

  const uint programID = compileShaders(InterpolatedVS, InterpolatedDepthPS);
  glUseProgram(programID);
  m_worldMatrixLocation = glGetUniformLocation(programID, "worldMatrix");
  assert(m_worldMatrixLocation != 0xFFFFFFFF);
  m_showDepthPosition = glGetUniformLocation(programID, "showDepth");
  assert(m_showDepthPosition != 0xFFFFFFFF);
  m_depthMapPosition = glGetUniformLocation(programID, "depthMap");
  assert(m_depthMapPosition != 0xFFFFFFFF);

  glGenTextures(1, &m_depthMapTexture);
  m_depthFramebuffer = createDepthBuffer(m_depthMapTexture);

  // create instance data
  uint instanceBuffer = createInstanceBuffer(vector<mat4>(4));
  std::vector<vec3> instancePositions({
    vec3(-0.5f, -0.5f, 0.5f), vec3(0.5f, -0.5f, -0.5f), vec3(0.5f, 0.5f, 0.25f),
    vec3(-0.5f, 0.5f, 0.75f),
  });

  uint objectBuffer = createVertexArray(
      vertices, vector<uint>{ 0, 1, 2, 2, 3, 0 }, instanceBuffer);

  while (!glfwWindowShouldClose(window))
  {
    // prepare
    std::vector<mat4> instanceData(4);

    static float scale = 0.2f;

    // scale += 0.01f;
    mat4 worldMatrix; // =
    // glm::scale(worldMatrix, vec3(sinf(scale), cosf(scale), atanf(scale)));
    worldMatrix = glm::scale(worldMatrix, vec3(scale, scale, scale));

    static float translation = 0.0f;
    translation += 0.01f;
    for (int i = 0; i < 4; ++i)
    {
      mat4 instDat;
      instDat = glm::translate(instDat, instancePositions[i]) *
                glm::translate(instDat, vec3(sinf(translation) * 0.2f * 1.4f,
                                             cosf(translation) * 0.2f, 0.0f));
      instanceData[i] = glm::transpose(instDat);
    }

    // bind instance buffer
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    // update instance buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * instanceData.size(),
                 instanceData.data(), GL_DYNAMIC_DRAW);

    // depth pass
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_depthFramebuffer);
    glUseProgram(shadowShader);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(m_worldMatrixShadowLocation, 1, GL_TRUE,
                       &worldMatrix[0][0]);

    glBindVertexArray(objectBuffer);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 4);
    glBindVertexArray(0);

    // draw pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(programID);
    // glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(m_worldMatrixLocation, 1, GL_TRUE, &worldMatrix[0][0]);
    if (OpenGLGame::showDepth)
    {
      glUniform1i(m_showDepthPosition, 1);
    }
    else
    {
      glUniform1i(m_showDepthPosition, 0);
    }

    glActiveTexture(m_depthMapPosition);
    glBindTexture(GL_TEXTURE_2D, m_depthMapPosition);

    glBindVertexArray(objectBuffer);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 4);
    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteBuffers(1, &instanceBuffer);
  glDeleteVertexArrays(1, &objectBuffer);
  glDeleteProgram(programID);

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}

void OpenGLGame::error_callback(int error, const char* description)
{

  cerr << description << endl;
}

void OpenGLGame::key_callback(GLFWwindow* window, int key, int scancode,
                              int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
  else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
  {
    if (OpenGLGame::showDepth)
    {
      OpenGLGame::showDepth = false;
    }
    else
    {
      OpenGLGame::showDepth = true;
    }
  }
}

const unsigned int OpenGLGame::createVertexArray(
    const std::vector<glm::vec3> vertices,
    const std::vector<unsigned int> indices, const uint instanceBuffer) const
{
  uint IDs[3];
  // vertexBuffer
  glGenBuffers(2, IDs);
  glBindBuffer(GL_ARRAY_BUFFER, IDs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(), vertices.data(),
               GL_STATIC_DRAW);
  // indexBuffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IDs[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(),
               indices.data(), GL_STATIC_DRAW);

  // vertexArray
  glGenVertexArrays(1, &IDs[2]);
  glBindVertexArray(IDs[2]);
  glBindBuffer(GL_ARRAY_BUFFER, IDs[0]);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribDivisor(0, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IDs[1]);
  glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
  for (uint i = 0; i < 4; ++i)
  {
    glEnableVertexAttribArray(1 + i);
    glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(mat4),
                          (const GLvoid*)(sizeof(float) * i * 4));
    glVertexAttribDivisor(1 + i, 1);
  }

  glBindVertexArray(GL_NONE);
  glDeleteBuffers(2, IDs);

  return IDs[2];
}

const uint OpenGLGame::createInstanceBuffer(const vector<mat4> instances) const
{
  uint result;
  glGenBuffers(1, &result);
  glBindBuffer(GL_ARRAY_BUFFER, result);
  glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(mat4),
               instances.data(), GL_DYNAMIC_DRAW);

  return result;
}

const uint OpenGLGame::createDepthBuffer(const uint textureId) const
{
  uint result;
  glGenFramebuffers(1, &result);

  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0,
               GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  glBindFramebuffer(GL_FRAMEBUFFER, result);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                         GL_TEXTURE_2D, textureId, 0);

  // Disable writes to the color buffer
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    cerr << "OpenGL FrameBuffer error, status: 0x" << status << endl;
    return 0;
  }

  return result;
}

} // namespace gl

} // namespace zge

#endif