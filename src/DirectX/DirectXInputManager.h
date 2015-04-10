#ifndef DIRECTXINPUTMANAGER_H
#define DIRECTXINPUTMANAGER_H

#include <windows.h>
#include "Preprocessors.h"

#include "IGameState.h"

#include "InputManager.h"

namespace zge
{

namespace dx
{

class DirectXInputManager : InputManager
{
public:
  static DirectXInputManager* getInstance();

  // destructor
  virtual ~DirectXInputManager() NOEXCEPT;
  // copy constructor
  DirectXInputManager(const DirectXInputManager&);
  // assignment operator
  DirectXInputManager& operator=(const DirectXInputManager&);
  // move constructor (C++11)
  DirectXInputManager(DirectXInputManager&&) NOEXCEPT;
  // move assignment operator (C++11)
  DirectXInputManager& operator=(DirectXInputManager&&) NOEXCEPT;

  static LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
  // constructor
  DirectXInputManager();

  void processKey(const EventType, const WPARAM) const;
  void processMouseMove(const LPARAM) const;
  void processMouseEvent(const Event&, const LPARAM) const;
  void processXMouseButtonEvent(const EventType, const WPARAM,
                                const LPARAM) const;

  // void processMouseButtonEvent(const EventType, )
};

} // namespace dx

} // namespace zge

#endif