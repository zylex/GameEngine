#ifndef IINPUTMANAGER_H
#define IINPUTMANAGER_H

#include "Preprocessors.h"

namespace zge
{
// forward declarations
class IInputManager
{
public:
  virtual ~IInputManager() NOEXCEPT = default;

  static IInputManager* getInstance();
  virtual const bool clear() = 0;
  virtual const bool enableKeyboard() = 0;
  virtual const bool enableMouse() = 0;
  // TODO: implement other inputs
  // virtual const bool enableTouch() = 0;
  // virtual const bool enableGamepad() = 0;
};

} // namespace zge

#endif