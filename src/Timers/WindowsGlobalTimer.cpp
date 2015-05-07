#if defined(_WIN64) || defined(_WIN32)
#include "WindowsGlobalTimer.h"

namespace zge
{
IGlobalTimer* IGlobalTimer::getInstance()
{
  return win::WindowsGlobalTimer::getInstance();
}

namespace win
{

WindowsGlobalTimer* WindowsGlobalTimer::getInstance()
{
  static WindowsGlobalTimer instance;
  return &instance;
}

WindowsGlobalTimer::WindowsGlobalTimer()
{
  // constructor
  QueryPerformanceFrequency(&this->frequency);
  QueryPerformanceCounter(&this->start);
}

WindowsGlobalTimer::~WindowsGlobalTimer() NOEXCEPT
{
  // destructor
}

WindowsGlobalTimer::WindowsGlobalTimer(const WindowsGlobalTimer& other)
{
  // copy constructor
}

WindowsGlobalTimer& WindowsGlobalTimer::operator=(const WindowsGlobalTimer& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

WindowsGlobalTimer::WindowsGlobalTimer(WindowsGlobalTimer&& other) NOEXCEPT
{
  // move constructor (C++11)
}

WindowsGlobalTimer& WindowsGlobalTimer::operator=(
    WindowsGlobalTimer&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new WindowsGlobalTimer(other));
}

const long double WindowsGlobalTimer::getElapsedSeconds()
{
  LARGE_INTEGER elapsedTime = this->getElapsedLargeInt();
  elapsedTime.QuadPart /= this->frequency.QuadPart;
  return (long double)elapsedTime.QuadPart;
}

const long long WindowsGlobalTimer::getElapsedNanoSeconds()
{
  LARGE_INTEGER elapsedTime = this->getElapsedLargeInt();
  elapsedTime.QuadPart *= TIMER_RESOLUTION;
  elapsedTime.QuadPart /= this->frequency.QuadPart;
  return (long long)elapsedTime.QuadPart;
}

const LARGE_INTEGER WindowsGlobalTimer::getElapsedLargeInt()
{
  LARGE_INTEGER end;
  QueryPerformanceCounter(&end);
  LARGE_INTEGER elapsedTime;
  elapsedTime.QuadPart = end.QuadPart - this->start.QuadPart;
  return elapsedTime;
}

} // namespace win

} // namespace zge

#endif
