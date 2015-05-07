#ifdef __unix__

#include "PosixGlobalTimer.h"

namespace zge
{

IGlobalTimer* IGlobalTimer::getInstance()
{
  return nix::PosixGlobalTimer::getInstance();
}

namespace nix
{

PosixGlobalTimer* PosixGlobalTimer::getInstance()
{
  static PosixGlobalTimer instance;
  return &instance;
}

PosixGlobalTimer::PosixGlobalTimer()
{
  // constructor
  clock_gettime(CLOCK_MONOTONIC, &this->start);
}

PosixGlobalTimer::~PosixGlobalTimer() noexcept
{
  // destructor
}

PosixGlobalTimer::PosixGlobalTimer(const PosixGlobalTimer& other)
{
  // copy constructor
}

PosixGlobalTimer& PosixGlobalTimer::operator=(const PosixGlobalTimer& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

PosixGlobalTimer::PosixGlobalTimer(PosixGlobalTimer&& other) noexcept
{
  // move constructor (C++11)
}

PosixGlobalTimer& PosixGlobalTimer::operator=(PosixGlobalTimer&& other) noexcept
{
  // move assignment operator (C++11)
  return *(new PosixGlobalTimer(other));
}

const long double PosixGlobalTimer::getElapsedSeconds()
{
  struct timespec elapsed = this->getElapsedTimespec();
  return elapsed.tv_sec + elapsed.tv_nsec / (long double)TIMER_RESOLUTION;
}

const long long PosixGlobalTimer::getElapsedNanoSeconds()
{
  struct timespec elapsed = this->getElapsedTimespec();
  return elapsed.tv_sec * TIMER_RESOLUTION + elapsed.tv_nsec;
}

const struct timespec PosixGlobalTimer::getElapsedTimespec()
{
  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC, &end);
  struct timespec elapsed;
  elapsed.tv_sec = end.tv_sec - this->start.tv_sec;
  elapsed.tv_nsec = end.tv_nsec - this->start.tv_nsec;
  if (elapsed.tv_nsec < 0)
  {
    --elapsed.tv_sec;
    elapsed.tv_nsec += TIMER_RESOLUTION;
  }
  return elapsed;
}

} // namespace nix

} // namespace zge

#endif
