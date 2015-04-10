#ifdef OPEN_GL
#include <iostream>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Preprocessors.h"

#include "OpenGLGame.h"

#include "OpenGLRenderer.h"
#include "OpenGLShader.h"
#include "OpenGLInputManager.h"
#include "OpenGLResourceManager.h"
#include "InterpolatedVS.h"
#include "InterpolatedDepthPS.h"
// #include "InterpolatedPS.h"

namespace zge
{

IGame* IGame::getInstance() { return gl::OpenGLGame::getInstance(); }

namespace gl
{

bool OpenGLGame::showDepth = false;

OpenGLGame* OpenGLGame::getInstance()
{
  static OpenGLGame instance;
  return &instance;
}

OpenGLGame::OpenGLGame() : AbstractGame()
{

  // constructor
}

OpenGLGame::~OpenGLGame() noexcept
{
  // destructor
  glfwDestroyWindow(this->window);
  glfwTerminate();
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

const bool OpenGLGame::initialise()
{
  if (not this->initialiseOpenGLWindow())
  {
    std::cerr << "Failed to initialise window." << std::endl;
    return false;
  }

  return AbstractGame::initialise();
}

const bool OpenGLGame::initialiseOpenGLWindow()
{
  // this can be done before initialising glfw
  glfwSetErrorCallback(OpenGLGame::error_callback);

  // init glfw
  if (not glfwInit())
  {
    std::cerr << "GLFW failed to initialise." << std::endl;
    return false;
  }

  // set the window hints
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // MAYBE: monitor chooser?
  // MAYBE: use primary or biggest monitor size for fullscreen
  // set the screen dimensions
  SCREEN_WIDTH(WINDOWED_WIDTH);
  SCREEN_HEIGHT(WINDOWED_HEIGHT);
  // create the window
  if (FULL_SCREEN)
  {
    this->window = glfwCreateWindow(SCREEN_WIDTH(), SCREEN_HEIGHT(),
                                    getWindowTitle().c_str(),
                                    glfwGetPrimaryMonitor(), nullptr);
  }
  else
  {
    this->window = glfwCreateWindow(SCREEN_WIDTH(), SCREEN_HEIGHT(),
                                    getWindowTitle().c_str(), nullptr, nullptr);
  }

  // make sure window was created
  if (not this->window)
  {
    std::cerr << "GLFW failed to initialise window." << std::endl;
    glfwTerminate();
    return false;
  }

  // make the window the open gl context
  glfwMakeContextCurrent(this->window);

  // init glew (and gl)
  glewExperimental = true;
  if (glewInit() IS_NOT GLEW_OK)
  {
    std::cerr << "GLEW failed to initialise." << std::endl;
    glfwTerminate();
    return false;
  }

  // this avoids errors maybe?
  // TODO: find out why this is here and correct this comment XD
  glfwSwapInterval(1);

  return true;
}

GLFWwindow* OpenGLGame::getWindow() const { return this->window; }

bool OpenGLGame::init()
{
  glfwSetErrorCallback(OpenGLGame::error_callback);

  if (not glfwInit())
  {
    std::cerr << "GLFW failed to initialise." << std::endl;
  }

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  SCREEN_WIDTH(WINDOWED_WIDTH);
  SCREEN_HEIGHT(WINDOWED_HEIGHT);
  if (not FULL_SCREEN)
  {
    this->window = glfwCreateWindow(SCREEN_WIDTH(), SCREEN_HEIGHT(),
                                    getWindowTitle().c_str(), nullptr, nullptr);
  }
  else
  {
    this->window = glfwCreateWindow(SCREEN_WIDTH(), SCREEN_HEIGHT(),
                                    getWindowTitle().c_str(),
                                    glfwGetPrimaryMonitor(), nullptr);
  }

  if (not this->window)
  {
    std::cerr << "GLFW failed to initialise window." << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(this->window);
  glewExperimental = true;
  if (glewInit() IS_NOT GLEW_OK)
  {
    std::cerr << "GLEW failed to initialise." << std::endl;
    glfwTerminate();
    return false;
  }

  glfwSwapInterval(1);
  glfwSetKeyCallback(this->window, OpenGLInputManager::key_callback);

  return true;
}

void OpenGLGame::frame()
{
  if (glfwWindowShouldClose(this->window))
  {
    this->quit();
  }
  else
  {
    AbstractGame::frame();
  }

  glfwSwapBuffers(this->window);
  glfwPollEvents();
}

const int OpenGLGame::run2()
{
  if (not init())
  {
    return EXIT_FAILURE;
  }

  // glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glEnable(GL_CULL_FACE);
  // glFrontFace(GL_CW);
  // glCullFace(GL_BACK);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  std::vector<glm::vec3> vertices;
  vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
  vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
  vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
  vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));

  std::vector<glm::vec3> normals;
  for (int i = 0; i < 4; ++i)
  {
    normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
  }

  const unsigned shadowShader = compileShader(InterpolatedVS);
  glUseProgram(shadowShader);
  _worldMatrixShadowLocation =
      glGetUniformLocation(shadowShader, "worldMatrix");
  assert(_worldMatrixShadowLocation IS_NOT 0xFFFFFFFF);

  const unsigned programID =
      compileShaders(InterpolatedVS, InterpolatedDepthPS);
  glUseProgram(programID);
  _worldMatrixLocation = glGetUniformLocation(programID, "worldMatrix");
  assert(_worldMatrixLocation IS_NOT 0xFFFFFFFF);
  _showDepthPosition = glGetUniformLocation(programID, "showDepth");
  assert(_showDepthPosition IS_NOT 0xFFFFFFFF);
  _depthMapPosition = glGetUniformLocation(programID, "depthMap");
  assert(_depthMapPosition IS_NOT 0xFFFFFFFF);

  OpenGLResourceManager* resourceManager = OpenGLResourceManager::getInstance();
  glGenTextures(1, &_depthMapTexture);
  _depthFramebuffer = resourceManager->createDepthBuffer(_depthMapTexture);

  // create instance data
  unsigned instanceBuffer =
      resourceManager->updateInstanceBuffer(std::vector<glm::mat4>(4));
  std::vector<glm::vec3> instancePositions({
      glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
      glm::vec3(0.5f, 0.5f, 0.25f), glm::vec3(-0.5f, 0.5f, 0.75f),
  });

  unsigned objectBuffer = resourceManager->createMesh(
      vertices, normals, std::vector<glm::uvec3>{ { 0, 1, 2 }, { 2, 3, 0 } });

#if defined(_DEBUG) || defined(DEBUG) || defined(NDEBUG)
  std::cout << "_depthMapPosition: " << _depthMapPosition << std::endl;
  std::cout << "_showDepthPosition: " << _showDepthPosition << std::endl;
  std::cout << "_worldMatrixShadowLocation: " << _worldMatrixShadowLocation
            << std::endl;
  std::cout << "_worldMatrixLocation: " << _worldMatrixLocation << std::endl;
#endif

  while (isRunning())
  {
    if (glfwWindowShouldClose(this->window))
    {
      quit();
    }
    else
    {
      // prepare
      std::vector<glm::mat4> instanceData(4);

      static float scale = 0.2f;

      // scale += 0.01f;
      glm::mat4 worldMatrix; // =
      // glm::scale(worldMatrix, glm::vec3(sinf(scale), cosf(scale),
      // atanf(scale)));
      worldMatrix = glm::scale(worldMatrix, glm::vec3(scale, scale, scale));

      static float translation = 0.0f;
      translation += 0.01f;
      for (int i = 0; i < 4; ++i)
      {
        glm::mat4 instDat;
        instDat =
            glm::translate(instDat, instancePositions[i]) *
            glm::translate(instDat, glm::vec3(sinf(translation) * 0.2f * 1.4f,
                                              cosf(translation) * 0.2f, 0.0f));
        instanceData[i] = glm::transpose(instDat);
      }

      // bind instance buffer
      glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
      // update instance buffer
      glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instanceData.size(),
                   instanceData.data(), GL_DYNAMIC_DRAW);

      // depth pass
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _depthFramebuffer);
      glUseProgram(shadowShader);
      glClear(GL_DEPTH_BUFFER_BIT);

      glUniformMatrix4fv(_worldMatrixShadowLocation, 1, GL_TRUE,
                         &worldMatrix[0][0]);

      glBindVertexArray(objectBuffer);
      glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 4);
      glBindVertexArray(0);

      // draw pass
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glUseProgram(programID);
      // glClear(GL_COLOR_BUFFER_BIT);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUniformMatrix4fv(_worldMatrixLocation, 1, GL_TRUE, &worldMatrix[0][0]);
      if (OpenGLGame::showDepth)
      {
        glUniform1i(_showDepthPosition, 1);
      }
      else
      {
        glUniform1i(_showDepthPosition, 0);
      }

      // glActiveTexture(_depthMapPosition);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, _depthMapPosition);

      glBindVertexArray(objectBuffer);
      glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 4);
      glBindVertexArray(0);

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
      glDisableVertexAttribArray(2);
      glDisableVertexAttribArray(3);
      glDisableVertexAttribArray(4);

      glfwSwapBuffers(this->window);
      glfwPollEvents();
    }
  }

  glDeleteBuffers(1, &instanceBuffer);
  glDeleteFramebuffers(1, &_depthFramebuffer);
  glDeleteVertexArrays(1, &objectBuffer);
  glDeleteProgram(programID);

  IInputManager::getInstance()->clear();

  // glDeleteVertexArrays(1, &vao);
  // glDeleteBuffers(1, &vbo);

  // glDeleteProgram(shader_program);

  glfwDestroyWindow(this->window);
  glfwTerminate();

  return EXIT_SUCCESS;
}

void OpenGLGame::error_callback(int error, const char* description)
{
  std::cerr << description << std::endl;
  std::cout << description << std::endl;
}

void OpenGLGame::key_callback(GLFWwindow*, const int, const int, const int,
                              const int)
{
}

} // namespace gl

} // namespace zge

#endif