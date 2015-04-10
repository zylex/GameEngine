#ifndef EVENT_H
#define EVENT_H

namespace zge
{

enum EventType
{
  EVENT_NULL,
  KEY_DOWN,
  KEY_UP,
  MOUSE_MOVE,
  MOUSE_BUTTON_DOWN,
  MOUSE_BUTTON_UP,
  MOUSE_SCROLL,
  ACCELEROMETER_MOVE,
  KINECT_MOVE
};

struct Event
{
  EventType eventType;
  int key;
  double x, y, z; // might need to be ints
  // for kinect we might want to know which joint moved or something like that
  int jointIndex;
};

enum Key
{
  // based on: http://www.glfw.org/docs/latest/group__keys.html
  ZGE_UNKNOWN,
  ZGE_SPACE,
  ZGE_QUOTE,  /* ' */
  ZGE_COMMA,  /* , */
  ZGE_MINUS,  /* - */
  ZGE_PERIOD, /* . */
  ZGE_SLASH,  /* / */
  ZGE_0,
  ZGE_1,
  ZGE_2,
  ZGE_3,
  ZGE_4,
  ZGE_5,
  ZGE_6,
  ZGE_7,
  ZGE_8,
  ZGE_9,
  ZGE_SEMICOLON, /* ; */
  ZGE_EQUAL,     /* = */
  ZGE_A,
  ZGE_B,
  ZGE_C,
  ZGE_D,
  ZGE_E,
  ZGE_F,
  ZGE_G,
  ZGE_H,
  ZGE_I,
  ZGE_J,
  ZGE_K,
  ZGE_L,
  ZGE_M,
  ZGE_N,
  ZGE_O,
  ZGE_P,
  ZGE_Q,
  ZGE_R,
  ZGE_S,
  ZGE_T,
  ZGE_U,
  ZGE_V,
  ZGE_W,
  ZGE_X,
  ZGE_Y,
  ZGE_Z,
  ZGE_LEFT_BRACKET,  /* [ */
  ZGE_BACKSLASH,     /* \ */
  ZGE_RIGHT_BRACKET, /* ] */
  ZGE_GRAVE_ACCENT,  /* ` */
  ZGE_WORLD_1,       /* non-US #1 */
  ZGE_WORLD_2,       /* non-US #2 */
  ZGE_ESCAPE,
  ZGE_ENTER,
  ZGE_TAB,
  ZGE_BACKSPACE,
  ZGE_INSERT,
  ZGE_DELETE,
  ZGE_ARROW_RIGHT,
  ZGE_ARROW_LEFT,
  ZGE_ARROW_DOWN,
  ZGE_ARROW_UP,
  ZGE_PAGE_UP,
  ZGE_PAGE_DOWN,
  ZGE_HOME,
  ZGE_END,
  ZGE_CAPS_LOCK,
  ZGE_SCROLL_LOCK,
  ZGE_NUM_LOCK,
  ZGE_PRINT_SCREEN,
  ZGE_PAUSE,
  ZGE_F1,
  ZGE_F2,
  ZGE_F3,
  ZGE_F4,
  ZGE_F5,
  ZGE_F6,
  ZGE_F7,
  ZGE_F8,
  ZGE_F9,
  ZGE_F10,
  ZGE_F11,
  ZGE_F12,
  ZGE_F13,
  ZGE_F14,
  ZGE_F15,
  ZGE_F16,
  ZGE_F17,
  ZGE_F18,
  ZGE_F19,
  ZGE_F20,
  ZGE_F21,
  ZGE_F22,
  ZGE_F23,
  ZGE_F24,
  ZGE_F25,
  ZGE_NUMPAD_0,
  ZGE_NUMPAD_1,
  ZGE_NUMPAD_2,
  ZGE_NUMPAD_3,
  ZGE_NUMPAD_4,
  ZGE_NUMPAD_5,
  ZGE_NUMPAD_6,
  ZGE_NUMPAD_7,
  ZGE_NUMPAD_8,
  ZGE_NUMPAD_9,
  ZGE_NUMPAD_DECIMAL,
  ZGE_NUMPAD_DIVIDE,
  ZGE_NUMPAD_MULTIPLY,
  ZGE_NUMPAD_SUBTRACT,
  ZGE_NUMPAD_ADD,
  ZGE_NUMPAD_ENTER,
  ZGE_NUMPAD_EQUAL,
  ZGE_LEFT_SHIFT,
  ZGE_LEFT_CONTROL,
  ZGE_LEFT_ALT,
  ZGE_LEFT_SUPER, /* win*/
  ZGE_RIGHT_SHIFT,
  ZGE_RIGHT_CONTROL,
  ZGE_RIGHT_ALT,
  ZGE_RIGHT_SUPER, /* win */
  ZGE_MENU,
  ZGE_LAST
};

enum MouseButton
{
  ZGE_MOUSE_UNKNOWN,
  ZGE_MOUSE_LEFT,
  ZGE_MOUSE_RIGHT,
  ZGE_MOUSE_MIDDLE,
  ZGE_MOUSE_FORWARD,
  ZGE_MOUSE_BACK,
  ZGE_MOUSE_6,
  ZGE_MOUSE_7,
  ZGE_MOUSE_8
};

} // namespace zge

#endif