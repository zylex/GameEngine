#include "AbstractGame.h"

namespace zge
{
const int SCREEN_WIDTH(const int width)
{
  static int screen_width = width;
  return screen_width;
}

const int SCREEN_HEIGHT(const int height)
{
  static int screen_height = height;
  return screen_height;
}
}