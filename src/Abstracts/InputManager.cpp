#include "InputManager.h"

namespace zge
{
InputManager::InputManager() : keyboardEnabled(true), mouseEnabled(true) {}

const bool InputManager::clear()
{
  this->keyboardEnabled = false;
  this->mouseEnabled = false;
  return true;
}

const bool InputManager::enableKeyboard()
{
  return this->keyboardEnabled = true;
}

const bool InputManager::disableKeyboard()
{
  return this->keyboardEnabled = false;
}

const bool InputManager::enableMouse() { return this->mouseEnabled = true; }

const bool InputManager::disableMouse() { return this->mouseEnabled = false; }

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
