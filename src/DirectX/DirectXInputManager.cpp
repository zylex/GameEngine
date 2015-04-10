#ifdef DIRECT_X
#include "IInputManager.h"
#include "DirectXInputManager.h"

namespace zge
{

IInputManager* IInputManager::getInstance()
{
  return dx::DirectXInputManager::getInstance();
}

namespace dx
{

DirectXInputManager* DirectXInputManager::getInstance()
{
  static DirectXInputManager instance;
  return &instance;
}

DirectXInputManager::DirectXInputManager()
{

  // constructor
}

DirectXInputManager::~DirectXInputManager() NOEXCEPT
{

  // destructor
}

DirectXInputManager::DirectXInputManager(const DirectXInputManager& other)
{

  // copy constructor
}

DirectXInputManager& DirectXInputManager::operator=(
    const DirectXInputManager& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

DirectXInputManager::DirectXInputManager(DirectXInputManager&& other) NOEXCEPT
{

  // move constructor (C++11)
}

DirectXInputManager& DirectXInputManager::operator=(
    DirectXInputManager&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new DirectXInputManager(other));
}

LRESULT CALLBACK DirectXInputManager::MessageHandler(HWND hWnd, UINT message,
                                                     WPARAM wParam,
                                                     LPARAM lParam)
{
  switch (message)
  {
    // Check if the window is being destroyed.
    case WM_DESTROY:
    {
      PostQuitMessage(0);
      return 0;
    }
    // Check if the window is being closed.
    case WM_CLOSE:
    {
      PostQuitMessage(0);
      return 0;
    }
  }

  if (keyboardIsEnabled())
  {
    switch (message)
    {
      case WM_KEYDOWN:
      {
        processKey(zge::KEY_DOWN, wParam);
      }
      case WM_KEYUP:
      {
        processKey(zge::KEY_UP, wParam);
        return 0;
      }
    }
  }

  if (mouseIsEnabled())
  {
    switch (message)
    {
      case WM_MOUSE_MOVE:
      {
        processMouseEvent({ zge::MOUSE_MOVE }, lParam);
        return 0;
      }
      case WM_LBUTTONDOWN:
      {
        processMouseEvent({ zge::MOUSE_BUTTON_DOWN, zge::MOUSE_LEFT }, lParam);
        return 0;
      }
      case WM_LBUTTONUP:
      {
        processMouseEvent({ zge::MOUSE_BUTTON_UP, zge::MOUSE_LEFT }, lParam);
        return 0;
      }
      case WM_MBUTTONDOWN:
      {
        processMouseEvent({ zge::MOUSE_BUTTON_DOWN, zge::MOUSE_MIDDLE },
                          lParam);
        return 0;
      }
      case WM_MBUTTONUP:
      {
        processMouseEvent({ zge::MOUSE_BUTTON_UP, zge::MOUSE_MIDDLE }, lParam);
        return 0;
      }
      case WM_RBUTTONDOWN:
      {
        processMouseEvent({ zge::MOUSE_BUTTON_DOWN, zge::MOUSE_RIGHT }, lParam);
        return 0;
      }
      case WM_RBUTTONUP:
      {
        processMouseEvent({ zge::MOUSE_BUTTON_UP, zge::MOUSE_RIGHT }, lParam);
        return 0;
      }
      case WM_MOUSEHWHEEL:
      {
        event.eventType = zge::MOUSE_SCROLL;
        event.x = GET_WHEEL_DELTA_WPARAM(wParam);
        return 0;
      }
      case WM_MOUSEWHEEL:
      {
        event.eventType = zge::MOUSE_SCROLL;
        event.y = GET_WHEEL_DELTA_WPARAM(wParam);
        return 0;
      }
      case WM_XBUTTONDOWN:
      {
        processXMouseButtonEvent(zge::MOUSE_BUTTON_DOWN, lParam, wParam);
        return true;
      }
      case WM_XBUTTONUP:
      {
        processXMouseButtonEvent(zge::MOUSE_BUTTON_UP, lParam, wParam);
        return true;
      }
    }
  }
  // forward the message to handler
  return DefWindowProc(hWnd, message, wParam, lParam);
}

void DirectXInputManager::processKey(const EventType eventType,
                                     const WPARAM wParam) const
{
  Event event;

  switch (wParam)
  {
    case VK_LBUTTON:
    {
      if (eventType IS zge::KEY_DOWN)
      {
        event.eventType = zge : MOUSE_BUTTON_DOWN;
      }
      else
      {
        event.eventType = zge : MOUSE_BUTTON_UP;
      }
      event.key = zge::MOUSE_LEFT;
      processMouseEvent(event, lParam);
      return;
    }
    case VK_RBUTTON:
    {
      if (eventType IS zge::KEY_DOWN)
      {
        event.eventType = zge : MOUSE_BUTTON_DOWN;
      }
      else
      {
        event.eventType = zge : MOUSE_BUTTON_UP;
      }
      event.key = zge::MOUSE_RIGHT;
      break;
    }
    case VK_MBUTTON:
    {
      if (eventType IS zge::KEY_DOWN)
      {
        event.eventType = zge : MOUSE_BUTTON_DOWN;
      }
      else
      {
        event.eventType = zge : MOUSE_BUTTON_UP;
      }
      event.eventType = zge : MOUSE_BUTTON_DOWN;
      event.key = zge::MOUSE_MIDDLE;
      break;
    }
    case VK_XBUTTON1:
    {
      if (eventType IS zge::KEY_DOWN)
      {
        event.eventType = zge : MOUSE_BUTTON_DOWN;
      }
      else
      {
        event.eventType = zge : MOUSE_BUTTON_UP;
      }
      event.eventType = zge : MOUSE_BUTTON_DOWN;
      event.key = zge::MOUSE_BACK;
      break;
    }
    case VK_XBUTTON2:
    {
      if (eventType IS zge::KEY_DOWN)
      {
        event.eventType = zge : MOUSE_BUTTON_DOWN;
      }
      else
      {
        event.eventType = zge : MOUSE_BUTTON_UP;
      }
      event.eventType = zge : MOUSE_BUTTON_DOWN;
      event.key = zge::MOUSE_FORWARD;
      break;
    }
    case VK_BACK:
    {
      event.key = zge::ZGE_BACKSPACE;
      break;
    }
    case VK_TAB:
    {
      event.key = zge::ZGE_TAB;
      break;
    }
    case VK_RETURN:
    {
      event.key = zge::ZGE_ENTER break;
    }
    case VK_SHIFT:
    {
      break;
    }
    case VK_CONTROL:
    {
      break;
    }

    case VK_MENU:
    {
      break;
    }
    case VK_PAUSE:
    {
      event.key = zge::ZGE_PAUSE;
      break;
    }
    case VK_CAPITAL:
    {
      event.key = zge::ZGE_CAPS_LOCK;
      break;
    }
    case VK_ESCAPE:
    {
      event.key = zge::ZGE_ESCAPE;
      break;
    }
    case VK_SPACE:
    {
      event.key = zge::ZGE_SPACE;
      break;
    }
    case VK_PRIOR:
    {
      event.key = zge::ZGE_PAGE_UP;
      break;
    }
    case VK_NEXT:
    {
      event.key = zge::ZGE_PAGE_DOWN;
      break;
    }
    case VK_END:
    {
      event.key = zge::ZGE_END;
      break;
    }
    case VK_HOME:
    {
      event.key = zge::ZGE_HOME;
      break;
    }
    case VK_LEFT:
    {
      event.key = zge::ZGE_ARROW_LEFT;
      break;
    }
    case VK_RIGHT:
    {
      event.key = zge::ZGE_ARROW_RIGHT;
      break;
    }
    case VK_UP:
    {
      event.key = zge::ZGE_ARROW_UP;
      break;
    }
    case VK_DOWN:
    {
      event.key = zge::ZGE_ARROW_DOWN;
      break;
    }
    case VK_SNAPSHOT:
    {
      event.key = zge::ZGE_PRINT_SCREEN;
      break;
    }
    case VK_INSERT:
    {
      event.key = zge::ZGE_INSERT;
      break;
    }
    case VK_DELETE:
    {
      event.key = zge::ZGE_DELETE;
      break;
    }
    case 0x30:
    {
      event.key = zge::ZGE_0;
      break;
    }
    case 0x31:
    {
      event.key = zge::ZGE_1;
      break;
    }
    case 0x32:
    {
      event.key = zge::ZGE_2;
      break;
    }
    case 0x33:
    {
      event.key = zge::ZGE_3;
      break;
    }
    case 0x34:
    {
      event.key = zge::ZGE_4;
      break;
    }
    case 0x35:
    {
      event.key = zge::ZGE_5;
      break;
    }
    case 0x36:
    {
      event.key = zge::ZGE_6;
      break;
    }
    case 0x37:
    {
      event.key = zge::ZGE_7;
      break;
    }
    case 0x38:
    {
      event.key = zge::ZGE_8;
      break;
    }
    case 0x39:
    {
      event.key = zge::ZGE_9;
      break;
    }
    case 0x41:
    {
      event.key = zge::ZGE_A;
      break;
    }
    case 0x42:
    {
      event.key = zge::ZGE_B;
      break;
    }
    case 0x43:
    {
      event.key = zge::ZGE_C;
      break;
    }
    case 0x44:
    {
      event.key = zge::ZGE_D;
      break;
    }
    case 0x45:
    {
      event.key = zge::ZGE_E;
      break;
    }
    case 0x46:
    {
      event.key = zge::ZGE_F;
      break;
    }
    case 0x47:
    {
      event.key = zge::ZGE_G;
      break;
    }
    case 0x48:
    {
      event.key = zge::ZGE_H;
      break;
    }
    case 0x49:
    {
      event.key = zge::ZGE_I;
      break;
    }
    case 0x4A:
    {
      event.key = zge::ZGE_J;
      break;
    }
    case 0x4B:
    {
      event.key = zge::ZGE_K;
      break;
    }
    case 0x4C:
    {
      event.key = zge::ZGE_L;
      break;
    }
    case 0x4D:
    {
      event.key = zge::ZGE_M;
      break;
    }
    case 0x4E:
    {
      event.key = zge::ZGE_N;
      break;
    }
    case 0x4F:
    {
      event.key = zge::ZGE_O;
      break;
    }
    case 0x50:
    {
      event.key = zge::ZGE_P;
      break;
    }
    case 0x51:
    {
      event.key = zge::ZGE_Q;
      break;
    }
    case 0x52:
    {
      event.key = zge::ZGE_R;
      break;
    }
    case 0x53:
    {
      event.key = zge::ZGE_S;
      break;
    }
    case 0x54:
    {
      event.key = zge::ZGE_T;
      break;
    }
    case 0x55:
    {
      event.key = zge::ZGE_U;
      break;
    }
    case 0x56:
    {
      event.key = zge::ZGE_V;
      break;
    }
    case 0x57:
    {
      event.key = zge::ZGE_W;
      break;
    }
    case 0x58:
    {
      event.key = zge::ZGE_X;
      break;
    }
    case 0x59:
    {
      event.key = zge::ZGE_Y;
      break;
    }
    case 0x5A:
    {
      event.key = zge::ZGE_Z;
      break;
    }
    case VK_LWIN:
    {
      event.key = zge::ZGE_LEFT_SUPER;
      break;
    }
    case VK_RWIN:
    {
      event.key = zge::ZGE_RIGHT_SUPER;
      break;
    }
    case VK_NUMPAD0:
    {
      event.key = zge::ZGE_NUMPAD_0;
      break;
    }
    case VK_NUMPAD1:
    {
      event.key = zge::ZGE_NUMPAD_1;
      break;
    }
    case VK_NUMPAD2:
    {
      event.key = zge::ZGE_NUMPAD_2;
      break;
    }
    case VK_NUMPAD3:
    {
      event.key = zge::ZGE_NUMPAD_3;
      break;
    }
    case VK_NUMPAD4:
    {
      event.key = zge::ZGE_NUMPAD_4;
      break;
    }
    case VK_NUMPAD5:
    {
      event.key = zge::ZGE_NUMPAD_5;
      break;
    }
    case VK_NUMPAD6:
    {
      event.key = zge::ZGE_NUMPAD_6;
      break;
    }
    case VK_NUMPAD7:
    {
      event.key = zge::ZGE_NUMPAD_7;
      break;
    }
    case VK_NUMPAD8:
    {
      event.key = zge::ZGE_NUMPAD_8;
      break;
    }
    case VK_NUMPAD9:
    {
      event.key = zge::ZGE_NUMPAD_9;
      break;
    }
    case VK_MULTIPLY:
    {
      event.key = zge::ZGE_NUMPAD_MULTIPLY;
      break;
    }
    case VK_ADD:
    {
      event.key = zge::ZGE_NUMPAD_ADD;
      break;
    }
    case VK_SUBTRACT:
    {
      event.key = zge::ZGE_NUMPAD_SUBTRACT;
      break;
    }
    case VK_DIVIDE:
    {
      event.key = zge::ZGE_NUMPAD_DIVIDE;
      break;
    }
    case VK_DECIMAL:
    {
      event.key = zge::ZGE_NUMPAD_DECIMAL;
      break;
    }
    case VK_F1:
    {
      event.key = zge::ZGE_F1;
      break;
    }
    case VK_F2:
    {
      event.key = zge::ZGE_F2;
      break;
    }
    case VK_F3:
    {
      event.key = zge::ZGE_F3;
      break;
    }
    case VK_F4:
    {
      event.key = zge::ZGE_F4;
      break;
    }
    case VK_F5:
    {
      event.key = zge::ZGE_F5;
      break;
    }
    case VK_F6:
    {
      event.key = zge::ZGE_F6;
      break;
    }
    case VK_F7:
    {
      event.key = zge::ZGE_F7;
      break;
    }
    case VK_F8:
    {
      event.key = zge::ZGE_F8;
      break;
    }
    case VK_F9:
    {
      event.key = zge::ZGE_F9;
      break;
    }
    case VK_F10:
    {
      event.key = zge::ZGE_F10;
      break;
    }
    case VK_F11:
    {
      event.key = zge::ZGE_F11;
      break;
    }
    case VK_F12:
    {
      event.key = zge::ZGE_F12;
      break;
    }
    case VK_F13:
    {
      event.key = zge::ZGE_F13;
      break;
    }
    case VK_F14:
    {
      event.key = zge::ZGE_F14;
      break;
    }
    case VK_F15:
    {
      event.key = zge::ZGE_F15;
      break;
    }
    case VK_F16:
    {
      event.key = zge::ZGE_F16;
      break;
    }
    case VK_F17:
    {
      event.key = zge::ZGE_F17;
      break;
    }
    case VK_F18:
    {
      event.key = zge::ZGE_F18;
      break;
    }
    case VK_F19:
    {
      event.key = zge::ZGE_F19;
      break;
    }
    case VK_F20:
    {
      event.key = zge::ZGE_F20;
      break;
    }
    case VK_F21:
    {
      event.key = zge::ZGE_F21;
      break;
    }
    case VK_F22:
    {
      event.key = zge::ZGE_F22;
      break;
    }
    case VK_F23:
    {
      event.key = zge::ZGE_F23;
      break;
    }
    case VK_F24:
    {
      event.key = zge::ZGE_F24;
      break;
    }
    case VK_NUMLOCK:
    {
      event.key = zge::ZGE_NUM_LOCK;
      break;
    }
    case VK_SCROLL:
    {
      event.key = zge::ZGE_SCROLL_LOCK;
      break;
    }
    case VK_LSHIFT:
    {
      event.key = zge::ZGE_LEFT_SHIFT;
      break;
    }
    case VK_RSHIFT:
    {
      event.key = zge::ZGE_RIGHT_SHIFT;
      break;
    }
    case VK_LCONTROL:
    {
      event.key = zge::ZGE_LEFT_CONTROL;
      break;
    }
    case VK_RCONTROL:
    {
      event.key = zge::ZGE_RIGHT_CONTROL;
      break;
    }
    case VK_OEM_1:
    {
      event.key = zge::ZGE_SEMICOLON;
      break;
    }
    case VK_OEM_PLUS:
    {
      event.key = zge::ZGE_EQUAL;
      break;
    }
    case VK_OEM_MINUS:
    {
      event.key = zge::ZGE_MINUS;
      break;
    }
    case VK_OEM_PERIOD:
    {
      event.key = zge::ZGE_PERIOD;
      break;
    }
    case VK_OEM_2:
    {
      event.key = zge::ZGE_SLASH;
      break;
    }
    case VK_OEM_3:
    {
      event.key = zge::ZGE_GRAVE_ACCENT;
      break;
    }
    case VK_OEM_4:
    {
      event.key = zge::ZGE_LEFT_BRACKET;
      break;
    }
    case VK_OEM_5:
    {
      event.key = zge::ZGE_BACKSLASH;
      break;
    }
    case VK_OEM_6:
    {
      event.key = zge::ZGE_RIGHT_BRACKET;
      break;
    }
    case VK_OEM_7:
    {
      event.key = zge::ZGE_QUOTE;
      break;
    }
    case VK_OEM_COMMA:
    {
      event.key = zge::ZGE_COMMA;
      break;
    }
    case VK_OEM_PERIOD:
    {
      event.key = zge::ZGE_PERIOD;
      break;
    }
    default:
    {
      event.key = zge::ZGE_UNKNOWN;
      break;
    }
  }

  event.eventType = eventType;
  IGame::getInstance()->getCurrentGameState()->processEvent(event);
}

void DirectXInputManager::processMouseEvent(const Event& event,
                                            const LPARAM lParam) const
{
  event.x = GET_X_LPARAM(lParam);
  event.y = GET_Y_LPARAM(lParam);
  IGame::getInstance()->getCurrentGameState()->processEvent(event);
}

void DirectXInputManager::processXMouseButtonEvent(const EventType eventType,
                                                   const WPARAM wParam,
                                                   const LPARAM lParam) const
{
  Event event;
  event.eventType = eventType;
  switch (HIWORD(wParam))
  {
    case XBUTTON1:
    {
      event.key = zge::ZGE_MOUSE_BACK;
      break;
    }
    case XBUTTON2:
    {
      event.key = zge::ZGE_MOUSE_FORWARD;
      break;
    }
    default:
    {
      event.key = zge::ZGE_UNKOWN;
    }
  }

  processMouseEvent(event, lParam);
}

} // namespace dx

} // namespace zge

#endif