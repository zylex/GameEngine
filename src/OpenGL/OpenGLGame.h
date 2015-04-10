#ifndef OPEN_GL_GAME_H
#define OPEN_GL_GAME_H

#include <vector>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Game.h"

namespace zge
{
namespace gl
{

class OpenGLGame : public Game
{
private:
  GLFWwindow* window;

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

  void frame();

  GLFWwindow* getWindow() const;

  static void error_callback(int, const char*);

  static bool showDepth;

private:
  // constructor
  OpenGLGame();
  const bool initialiseOpenGLWindow();
};

} // namespace gl

} // namespace zge

#endif