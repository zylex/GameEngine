#ifndef WINDOWSGLOBALTIMER_H
#define WINDOWSGLOBALTIMER_H

#include <windows.h>

#include "Preprocessors.h"

#include "IGlobalTimer.h"

namespace zge
{

namespace win
{

class WindowsGlobalTimer : public IGlobalTimer
{
private:
  LARGE_INTEGER start, frequency;

public:
  static WindowsGlobalTimer* getInstance();

  // destructor
  virtual ~WindowsGlobalTimer() NOEXCEPT;
  // copy constructor
  WindowsGlobalTimer(const WindowsGlobalTimer&);
  // assignment operator
  WindowsGlobalTimer& operator=(const WindowsGlobalTimer&);
  // move constructor (C++11)
  WindowsGlobalTimer(WindowsGlobalTimer&&) NOEXCEPT;
  // move assignment operator (C++11)
  WindowsGlobalTimer& operator=(WindowsGlobalTimer&&) NOEXCEPT;

  const long double getElapsedSeconds();
  const long long getElapsedNanoSeconds();

private:
  // constructor
  WindowsGlobalTimer();

  const LARGE_INTEGER getElapsedLargeInt();
};

} // namespace win

} // namespace zge

#endif