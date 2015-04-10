#ifdef OPEN_GL
#include "OpenGLGame.h"
#ifdef USE_ANT
#include <AntTweakBar.h>
#endif

#include "OpenGLInputManager.h"

#include "Event.h"

namespace zge
{

IInputManager* IInputManager::getInstance()
{
  return gl::OpenGLInputManager::getInstance();
}

namespace gl
{

OpenGLInputManager* OpenGLInputManager::getInstance()
{
  static OpenGLInputManager instance;
  return &instance;
}

OpenGLInputManager::OpenGLInputManager()
{

  // constructor
}

OpenGLInputManager::~OpenGLInputManager() noexcept
{

  // destructor
}

OpenGLInputManager::OpenGLInputManager(const OpenGLInputManager& other)
{

  // copy constructor
}

OpenGLInputManager& OpenGLInputManager::operator=(const OpenGLInputManager& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

OpenGLInputManager::OpenGLInputManager(OpenGLInputManager&& other) noexcept
{

  // move constructor (C++11)
}

OpenGLInputManager& OpenGLInputManager::operator=(
    OpenGLInputManager&& other) noexcept
{
  // move assignment operator (C++11)
  return *(new OpenGLInputManager(other));
}

const bool OpenGLInputManager::clear()
{
  GLFWwindow* window = OpenGLGame::getInstance()->getWindow();
  if (this->keyboardIsEnabled())
  {
#ifdef USE_ANT
    glfwSetKeyCallback(window, OpenGLInputManager::key_callback);

#else
    glfwSetKeyCallback(OpenGLGame::getInstance()->getWindow(), nullptr);
#endif

    this->setKeyboardEnabled(false);
  }

  if (this->mouseIsEnabled())
  {
#ifdef USE_ANT
    glfwSetMouseButtonCallback(OpenGLGame::getInstance()->getWindow(),
                               OpenGLInputManager::mouse_button_callback);
    glfwSetCursorPosCallback(OpenGLGame::getInstance()->getWindow(),
                             OpenGLInputManager::mouse_position_callback);
    glfwSetScrollCallback(OpenGLGame::getInstance()->getWindow(),
                          OpenGLInputManager::mouse_scroll_callback);
#else
    glfwSetMouseButtonCallback(OpenGLGame::getInstance()->getWindow(), nullptr);
    glfwSetCursorPosCallback(OpenGLGame::getInstance()->getWindow(), nullptr);
    glfwSetScrollCallback(OpenGLGame::getInstance()->getWindow(), nullptr);
#endif

    this->setMouseEnabled(false);
  }

  return not(this->keyboardIsEnabled() and this->mouseIsEnabled());
}

const bool OpenGLInputManager::enableKeyboard()
{
  if (not keyboardIsEnabled()
#ifndef USE_ANT
      and glfwSetKeyCallback(OpenGLGame::getInstance()->getWindow(),
                             OpenGLInputManager::key_callback)
#endif
          )
  {
    return InputManager::enableKeyboard();
  }
  else
  {
    return false;
  }
}

const bool OpenGLInputManager::enableMouse()
{
  if (not mouseIsEnabled()
#ifndef USE_ANT
      and
      glfwSetMouseButtonCallback(OpenGLGame::getInstance()->getWindow(),
                                 OpenGLInputManager::mouse_button_callback) and
      glfwSetCursorPosCallback(OpenGLGame::getInstance()->getWindow(),
                               OpenGLInputManager::mouse_position_callback) and
      glfwSetScrollCallback(OpenGLGame::getInstance()->getWindow(),
                            OpenGLInputManager::mouse_scroll_callback)
#endif
          )
  {
    return InputManager::enableMouse();
  }
  else
  {
    return false;
  }
}

void OpenGLInputManager::keyCallback(GLFWwindow* window, const int key,
                                     const int scancode, const int action,
                                     const int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
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
#ifdef USE_ANT
  if (TwEventKeyGLFW(key, action))
  {
    return;
  }
#endif
  if (keyboardIsEnabled())
  {
    Event event;

    switch (key)
    {
      case GLFW_KEY_SPACE:
        event.key = zge::ZGE_SPACE;
        break;
      case GLFW_KEY_APOSTROPHE:
        event.key = zge::ZGE_QUOTE;
        break;
      case GLFW_KEY_COMMA:
        event.key = zge::ZGE_COMMA;
        break;
      case GLFW_KEY_MINUS:
        event.key = zge::ZGE_MINUS;
        break;
      case GLFW_KEY_PERIOD:
        event.key = zge::ZGE_PERIOD;
        break;
      case GLFW_KEY_SLASH:
        event.key = zge::ZGE_SLASH;
        break;
      case GLFW_KEY_0:
        event.key = zge::ZGE_0;
        break;
      case GLFW_KEY_1:
        event.key = zge::ZGE_1;
        break;
      case GLFW_KEY_2:
        event.key = zge::ZGE_2;
        break;
      case GLFW_KEY_3:
        event.key = zge::ZGE_3;
        break;
      case GLFW_KEY_4:
        event.key = zge::ZGE_4;
        break;
      case GLFW_KEY_5:
        event.key = zge::ZGE_5;
        break;
      case GLFW_KEY_6:
        event.key = zge::ZGE_6;
        break;
      case GLFW_KEY_7:
        event.key = zge::ZGE_7;
        break;
      case GLFW_KEY_8:
        event.key = zge::ZGE_8;
        break;
      case GLFW_KEY_9:
        event.key = zge::ZGE_9;
        break;
      case GLFW_KEY_SEMICOLON:
        event.key = zge::ZGE_SEMICOLON;
        break;
      case GLFW_KEY_EQUAL:
        event.key = zge::ZGE_EQUAL;
        break;
      case GLFW_KEY_A:
        event.key = zge::ZGE_A;
        break;
      case GLFW_KEY_B:
        event.key = zge::ZGE_B;
        break;
      case GLFW_KEY_C:
        event.key = zge::ZGE_C;
        break;
      case GLFW_KEY_D:
        event.key = zge::ZGE_D;
        break;
      case GLFW_KEY_E:
        event.key = zge::ZGE_E;
        break;
      case GLFW_KEY_F:
        event.key = zge::ZGE_F;
        break;
      case GLFW_KEY_G:
        event.key = zge::ZGE_G;
        break;
      case GLFW_KEY_H:
        event.key = zge::ZGE_H;
        break;
      case GLFW_KEY_I:
        event.key = zge::ZGE_I;
        break;
      case GLFW_KEY_J:
        event.key = zge::ZGE_J;
        break;
      case GLFW_KEY_K:
        event.key = zge::ZGE_K;
        break;
      case GLFW_KEY_L:
        event.key = zge::ZGE_L;
        break;
      case GLFW_KEY_M:
        event.key = zge::ZGE_M;
        break;
      case GLFW_KEY_N:
        event.key = zge::ZGE_N;
        break;
      case GLFW_KEY_O:
        event.key = zge::ZGE_O;
        break;
      case GLFW_KEY_P:
        event.key = zge::ZGE_P;
        break;
      case GLFW_KEY_Q:
        event.key = zge::ZGE_Q;
        break;
      case GLFW_KEY_R:
        event.key = zge::ZGE_R;
        break;
      case GLFW_KEY_S:
        event.key = zge::ZGE_S;
        break;
      case GLFW_KEY_T:
        event.key = zge::ZGE_T;
        break;
      case GLFW_KEY_U:
        event.key = zge::ZGE_U;
        break;
      case GLFW_KEY_V:
        event.key = zge::ZGE_V;
        break;
      case GLFW_KEY_W:
        event.key = zge::ZGE_W;
        break;
      case GLFW_KEY_X:
        event.key = zge::ZGE_X;
        break;
      case GLFW_KEY_Y:
        event.key = zge::ZGE_Y;
        break;
      case GLFW_KEY_Z:
        event.key = zge::ZGE_Z;
        break;
      case GLFW_KEY_LEFT_BRACKET:
        event.key = zge::ZGE_LEFT_BRACKET;
        break;
      case GLFW_KEY_BACKSLASH:
        event.key = zge::ZGE_BACKSLASH;
        break;
      case GLFW_KEY_RIGHT_BRACKET:
        event.key = zge::ZGE_RIGHT_BRACKET;
        break;
      case GLFW_KEY_GRAVE_ACCENT:
        event.key = zge::ZGE_GRAVE_ACCENT;
        break;
      case GLFW_KEY_WORLD_1:
        event.key = zge::ZGE_WORLD_1;
        break;
      case GLFW_KEY_WORLD_2:
        event.key = zge::ZGE_WORLD_2;
        break;
      case GLFW_KEY_ESCAPE:
        event.key = zge::ZGE_ESCAPE;
        break;
      case GLFW_KEY_ENTER:
        event.key = zge::ZGE_ENTER;
        break;
      case GLFW_KEY_TAB:
        event.key = zge::ZGE_TAB;
        break;
      case GLFW_KEY_BACKSPACE:
        event.key = zge::ZGE_BACKSPACE;
        break;
      case GLFW_KEY_INSERT:
        event.key = zge::ZGE_INSERT;
        break;
      case GLFW_KEY_DELETE:
        event.key = zge::ZGE_DELETE;
        break;
      case GLFW_KEY_RIGHT:
        event.key = zge::ZGE_ARROW_RIGHT;
        break;
      case GLFW_KEY_LEFT:
        event.key = zge::ZGE_ARROW_LEFT;
        break;
      case GLFW_KEY_DOWN:
        event.key = zge::ZGE_ARROW_DOWN;
        break;
      case GLFW_KEY_UP:
        event.key = zge::ZGE_ARROW_UP;
        break;
      case GLFW_KEY_PAGE_UP:
        event.key = zge::ZGE_PAGE_UP;
        break;
      case GLFW_KEY_PAGE_DOWN:
        event.key = zge::ZGE_PAGE_DOWN;
        break;
      case GLFW_KEY_HOME:
        event.key = zge::ZGE_HOME;
        break;
      case GLFW_KEY_END:
        event.key = zge::ZGE_END;
        break;
      case GLFW_KEY_CAPS_LOCK:
        event.key = zge::ZGE_CAPS_LOCK;
        break;
      case GLFW_KEY_SCROLL_LOCK:
        event.key = zge::ZGE_SCROLL_LOCK;
        break;
      case GLFW_KEY_NUM_LOCK:
        event.key = zge::ZGE_NUM_LOCK;
        break;
      case GLFW_KEY_PRINT_SCREEN:
        event.key = zge::ZGE_PRINT_SCREEN;
        break;
      case GLFW_KEY_PAUSE:
        event.key = zge::ZGE_PAUSE;
        break;
      case GLFW_KEY_F1:
        event.key = zge::ZGE_F1;
        break;
      case GLFW_KEY_F2:
        event.key = zge::ZGE_F2;
        break;
      case GLFW_KEY_F3:
        event.key = zge::ZGE_F3;
        break;
      case GLFW_KEY_F4:
        event.key = zge::ZGE_F4;
        break;
      case GLFW_KEY_F5:
        event.key = zge::ZGE_F5;
        break;
      case GLFW_KEY_F6:
        event.key = zge::ZGE_F6;
        break;
      case GLFW_KEY_F7:
        event.key = zge::ZGE_F7;
        break;
      case GLFW_KEY_F8:
        event.key = zge::ZGE_F8;
        break;
      case GLFW_KEY_F9:
        event.key = zge::ZGE_F9;
        break;
      case GLFW_KEY_F10:
        event.key = zge::ZGE_F10;
        break;
      case GLFW_KEY_F11:
        event.key = zge::ZGE_F11;
        break;
      case GLFW_KEY_F12:
        event.key = zge::ZGE_F12;
        break;
      case GLFW_KEY_F13:
        event.key = zge::ZGE_F13;
        break;
      case GLFW_KEY_F14:
        event.key = zge::ZGE_F14;
        break;
      case GLFW_KEY_F15:
        event.key = zge::ZGE_F15;
        break;
      case GLFW_KEY_F16:
        event.key = zge::ZGE_F16;
        break;
      case GLFW_KEY_F17:
        event.key = zge::ZGE_F17;
        break;
      case GLFW_KEY_F18:
        event.key = zge::ZGE_F18;
        break;
      case GLFW_KEY_F19:
        event.key = zge::ZGE_F19;
        break;
      case GLFW_KEY_F20:
        event.key = zge::ZGE_F20;
        break;
      case GLFW_KEY_F21:
        event.key = zge::ZGE_F21;
        break;
      case GLFW_KEY_F22:
        event.key = zge::ZGE_F22;
        break;
      case GLFW_KEY_F23:
        event.key = zge::ZGE_F23;
        break;
      case GLFW_KEY_F24:
        event.key = zge::ZGE_F24;
        break;
      case GLFW_KEY_F25:
        event.key = zge::ZGE_F25;
        break;
      case GLFW_KEY_KP_0:
        event.key = zge::ZGE_NUMPAD_0;
        break;
      case GLFW_KEY_KP_1:
        event.key = zge::ZGE_NUMPAD_1;
        break;
      case GLFW_KEY_KP_2:
        event.key = zge::ZGE_NUMPAD_2;
        break;
      case GLFW_KEY_KP_3:
        event.key = zge::ZGE_NUMPAD_3;
        break;
      case GLFW_KEY_KP_4:
        event.key = zge::ZGE_NUMPAD_4;
        break;
      case GLFW_KEY_KP_5:
        event.key = zge::ZGE_NUMPAD_5;
        break;
      case GLFW_KEY_KP_6:
        event.key = zge::ZGE_NUMPAD_6;
        break;
      case GLFW_KEY_KP_7:
        event.key = zge::ZGE_NUMPAD_7;
        break;
      case GLFW_KEY_KP_8:
        event.key = zge::ZGE_NUMPAD_8;
        break;
      case GLFW_KEY_KP_9:
        event.key = zge::ZGE_NUMPAD_9;
        break;
      case GLFW_KEY_KP_DECIMAL:
        event.key = zge::ZGE_NUMPAD_DECIMAL;
        break;
      case GLFW_KEY_KP_DIVIDE:
        event.key = zge::ZGE_NUMPAD_DIVIDE;
        break;
      case GLFW_KEY_KP_MULTIPLY:
        event.key = zge::ZGE_NUMPAD_MULTIPLY;
        break;
      case GLFW_KEY_KP_SUBTRACT:
        event.key = zge::ZGE_NUMPAD_SUBTRACT;
        break;
      case GLFW_KEY_KP_ADD:
        event.key = zge::ZGE_NUMPAD_ADD;
        break;
      case GLFW_KEY_KP_ENTER:
        event.key = zge::ZGE_NUMPAD_ENTER;
        break;
      case GLFW_KEY_KP_EQUAL:
        event.key = zge::ZGE_NUMPAD_EQUAL;
        break;
      case GLFW_KEY_LEFT_SHIFT:
        event.key = zge::ZGE_LEFT_SHIFT;
        break;
      case GLFW_KEY_LEFT_CONTROL:
        event.key = zge::ZGE_LEFT_CONTROL;
        break;
      case GLFW_KEY_LEFT_ALT:
        event.key = zge::ZGE_LEFT_ALT;
        break;
      case GLFW_KEY_LEFT_SUPER:
        event.key = zge::ZGE_LEFT_SUPER;
        break;
      case GLFW_KEY_RIGHT_SHIFT:
        event.key = zge::ZGE_RIGHT_SHIFT;
        break;
      case GLFW_KEY_RIGHT_CONTROL:
        event.key = zge::ZGE_RIGHT_CONTROL;
        break;
      case GLFW_KEY_RIGHT_ALT:
        event.key = zge::ZGE_RIGHT_ALT;
        break;
      case GLFW_KEY_RIGHT_SUPER:
        event.key = zge::ZGE_RIGHT_SUPER;
        break;
      case GLFW_KEY_MENU:
        event.key = zge::ZGE_MENU;
        break;
      default:
        event.key = zge::ZGE_UNKNOWN;
    }

    switch (action)
    {
      case GLFW_PRESS:
        event.eventType = zge::KEY_DOWN;
        break;
      case GLFW_RELEASE:
        event.eventType = zge::KEY_UP;
        break;
      default:
        event.eventType = zge::EVENT_NULL;
    }

    IGame::getInstance()->getCurrentGameState()->processEvent(event);
  }
}

void OpenGLInputManager::mouseButtonCallback(GLFWwindow* window,
                                             const int button, const int action,
                                             const int mods)
{

#ifdef USE_ANT
  if (TwEventMouseButtonGLFW(button, action))
  {
    return;
  }
#endif
  if (mouseIsEnabled())
  {
    Event event;

    switch (action)
    {
      case GLFW_PRESS:
        event.eventType = zge::MOUSE_BUTTON_DOWN;
        break;
      case GLFW_RELEASE:
        event.eventType = zge::MOUSE_BUTTON_UP;
        break;
      default:
        event.eventType = zge::EVENT_NULL;
    }

    switch (button)
    {
      case GLFW_MOUSE_BUTTON_LEFT:
      {
        event.key = zge::ZGE_MOUSE_LEFT;
        break;
      }
      case GLFW_MOUSE_BUTTON_RIGHT:
      {
        event.key = zge::ZGE_MOUSE_RIGHT;
        break;
      }
      case GLFW_MOUSE_BUTTON_MIDDLE:
      {
        event.key = zge::ZGE_MOUSE_MIDDLE;
        break;
      }
      case GLFW_MOUSE_BUTTON_4:
      {
        event.key = zge::ZGE_MOUSE_BACK;
        break;
      }
      case GLFW_MOUSE_BUTTON_5:
      {
        event.key = zge::ZGE_MOUSE_FORWARD;
        break;
      }
      case GLFW_MOUSE_BUTTON_6:
      {
        event.key = zge::ZGE_MOUSE_6;
        break;
      }
      case GLFW_MOUSE_BUTTON_7:
      {
        event.key = zge::ZGE_MOUSE_7;
        break;
      }
      case GLFW_MOUSE_BUTTON_8:
      {
        event.key = zge::ZGE_MOUSE_8;
        break;
      }
      default:
        event.key = zge::ZGE_UNKNOWN;
        break;
    }

    glfwGetCursorPos(window, &event.x, &event.y);

    IGame::getInstance()->getCurrentGameState()->processEvent(event);
  }
}

void OpenGLInputManager::mousePositionCallback(GLFWwindow* window,
                                               const double xPos,
                                               const double yPos)
{

#ifdef USE_ANT
  if (TwEventMousePosGLFW(xPos, yPos))
  {
    return;
  }
#endif
  if (mouseIsEnabled())
  {
    Event event;

    event.eventType = zge::MOUSE_MOVE;
    event.x = xPos;
    event.y = yPos;

    IGame::getInstance()->getCurrentGameState()->processEvent(event);
  }
}

void OpenGLInputManager::mouseScrollCallback(GLFWwindow* window,
                                             const double xOffset,
                                             const double yOffset)
{

#ifdef USE_ANT
  if (TwEventMouseWheelGLFW(xOffset))
  {
    return;
  }
#endif
  if (mouseIsEnabled())
  {

    Event event;

    event.eventType = zge::MOUSE_SCROLL;
    event.x = xOffset;
    event.y = yOffset;

    IGame::getInstance()->getCurrentGameState()->processEvent(event);
  }
}

void OpenGLInputManager::key_callback(GLFWwindow* window, int key, int scancode,
                                      int action, int mods)
{
  OpenGLInputManager::getInstance()->keyCallback(window, key, scancode, action,
                                                 mods);
}

void OpenGLInputManager::mouse_button_callback(GLFWwindow* window, int button,
                                               int action, int mods)
{
  OpenGLInputManager::getInstance()->mouseButtonCallback(window, button, action,
                                                         mods);
}

void OpenGLInputManager::mouse_position_callback(GLFWwindow* window,
                                                 double xPos, double yPos)
{
  OpenGLInputManager::getInstance()->mousePositionCallback(window, xPos, yPos);
}

void OpenGLInputManager::mouse_scroll_callback(GLFWwindow* window,
                                               double xOffset, double yOffset)
{
  OpenGLInputManager::getInstance()->mouseScrollCallback(window, xOffset,
                                                         yOffset);
}

} // namespace gl

} // namespace zge

#endif