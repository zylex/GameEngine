#include "InputManager.h"

namespace zge
{
InputManager::InputManager() : keyboardEnabled(true), mouseEnabled(true) {}

const bool InputManager::enableKeyboard()
{
  return this->keyboardEnabled = true;
}

const bool InputManager::enableMouse() { return this->mouseEnabled = true; }

void InputManager::setKeyboardEnabled(const bool keyboardEnabled)
{
  this->keyboardEnabled = keyboardEnabled;
}

const bool InputManager::keyboardIsEnabled() { return this->keyboardEnabled; }

void InputManager::setMouseEnabled(const bool mouseEnabled)
{
  this->mouseEnabled = mouseEnabled;
}

const bool InputManager::mouseIsEnabled() { return this->mouseEnabled; }

} // namespace zge