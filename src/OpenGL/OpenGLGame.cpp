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

OpenGLGame::OpenGLGame() : Game()
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

  return Game::initialise();
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
                                    this->getWindowTitle().c_str(),
                                    glfwGetPrimaryMonitor(), nullptr);
  }
  else
  {
    this->window =
        glfwCreateWindow(SCREEN_WIDTH(), SCREEN_HEIGHT(),
                         this->getWindowTitle().c_str(), nullptr, nullptr);
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

void OpenGLGame::frame()
{
  if (glfwWindowShouldClose(this->window))
  {
    this->quit();
  }
  else
  {
    Game::frame();
  }

  glfwSwapBuffers(this->window);
  glfwPollEvents();
}

void OpenGLGame::error_callback(int error, const char* description)
{
  std::cerr << description << std::endl;
  std::cout << description << std::endl;
}

} // namespace gl

} // namespace zge

#endif
