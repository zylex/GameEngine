#ifndef OPENGLINPUTMANAGER_H
#define OPENGLINPUTMANAGER_H

#include <GLFW/glfw3.h>

#include "InputManager.h"

namespace zge
{

namespace gl
{

class OpenGLInputManager : public InputManager
{
public:
  static OpenGLInputManager* getInstance();

  static void key_callback(GLFWwindow*, int, int, int, int);
  static void mouse_button_callback(GLFWwindow*, int, int, int);
  static void mouse_position_callback(GLFWwindow*, double, double);
  static void mouse_scroll_callback(GLFWwindow*, double, double);

  // destructor
  virtual ~OpenGLInputManager() noexcept;
  // copy constructor
  OpenGLInputManager(const OpenGLInputManager&);
  // assignment operator
  OpenGLInputManager& operator=(const OpenGLInputManager&);
  // move constructor (C++11)
  OpenGLInputManager(OpenGLInputManager&&) noexcept;
  // move assignment operator (C++11)
  OpenGLInputManager& operator=(OpenGLInputManager&&) noexcept;

  const bool init();
  const bool clear();
  const bool enableKeyboard();
  const bool enableMouse();

  void keyCallback(GLFWwindow*, const int, const int, const int, const int);
  void mouseButtonCallback(GLFWwindow*, const int, const int, const int);
  void mousePositionCallback(GLFWwindow*, const double, const double);
  void mouseScrollCallback(GLFWwindow*, const double, const double);

private:
  // constructor
  OpenGLInputManager();
};

} // namespace gl

} // namespace zge

#endif
