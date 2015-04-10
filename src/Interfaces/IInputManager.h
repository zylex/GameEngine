#ifndef IINPUTMANAGER_H
#define IINPUTMANAGER_H

#include "Preprocessors.h"

namespace zge
{

class IInputManager
{
public:
  virtual ~IInputManager() NOEXCEPT = default;

  static IInputManager* getInstance();
  virtual const bool clear() = 0;
  virtual const bool enableKeyboard() = 0;
  // virtual const bool disableKeyboard() = 0;
  virtual const bool enableMouse() = 0;
  // virtual const bool disableMouse() = 0;
  // virtual const bool enableTouch() = 0;
  // virtual const bool enableGamepad() = 0;
};

} // namespace zge

#endif