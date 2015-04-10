#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Preprocessors.h"
#include "IInputManager.h"
#include "IGameState.h"

namespace zge
{

class InputManager : public IInputManager
{
private:
  bool keyboardEnabled;
  bool mouseEnabled;
  IGameState* currentState;

public:
  InputManager();
  virtual ~InputManager() NOEXCEPT = default;

  const bool enableKeyboard();
  const bool enableMouse();
  void setKeyboardEnabled(const bool);
  const bool keyboardIsEnabled();
  void setMouseEnabled(const bool);
  const bool mouseIsEnabled();
  void setCurrentGameState(IGameState*);
};

} // namespace zge

#endif