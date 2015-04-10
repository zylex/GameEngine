#ifndef OPEN_GL_GAME_H
#define OPEN_GL_GAME_H

#include <vector>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "AbstractGame.h"

namespace zge
{
namespace gl
{

class OpenGLGame : public AbstractGame
{
private:
  GLFWwindow* window;

  unsigned _worldMatrixLocation;
  unsigned _worldMatrixShadowLocation;
  unsigned _showDepthPosition;
  unsigned _depthMapPosition;
  unsigned _depthMapTexture;
  unsigned _depthFramebuffer;

public:
  static OpenGLGame* getInstance();
  // destructor
  virtual ~OpenGLGame() noexcept;
  // copy constructor
  OpenGLGame(const OpenGLGame& other);
  // assignment operator
  OpenGLGame& operator=(const OpenGLGame& other);
  // move constructor (C++11)
  OpenGLGame(OpenGLGame&& other) noexcept;
  // move assignment operator (C++11)
  OpenGLGame& operator=(OpenGLGame&& other) noexcept;

  const bool initialise();

  // const int run();
  void frame();

  const int run2();

  GLFWwindow* getWindow() const;

  static void error_callback(int, const char*);

  static bool showDepth;

  static void key_callback(GLFWwindow*, int, int, int, int);

private:
  // constructor
  OpenGLGame();
  bool init();
  const bool initialiseOpenGLWindow();
};

} // namespace gl

} // namespace zge

#endif