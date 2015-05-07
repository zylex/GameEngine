#ifndef POSIXGLOBALTIMER_H
#define POSIXGLOBALTIMER_H

#include <time.h>

#include "IGlobalTimer.h"

namespace zge
{

namespace nix
{

class PosixGlobalTimer : public IGlobalTimer
{
private:
  // timeval start, end, elapsed;
  struct timespec start;

public:
  static PosixGlobalTimer* getInstance();

  // destructor
  virtual ~PosixGlobalTimer() noexcept;
  // copy constructor
  PosixGlobalTimer(const PosixGlobalTimer&);
  // assignment operator
  PosixGlobalTimer& operator=(const PosixGlobalTimer&);
  // move constructor (C++11)
  PosixGlobalTimer(PosixGlobalTimer&&) noexcept;
  // move assignment operator (C++11)
  PosixGlobalTimer& operator=(PosixGlobalTimer&&) noexcept;

  const long double getElapsedSeconds();
  const long long getElapsedNanoSeconds();

private:
  // constructor
  PosixGlobalTimer();

  const struct timespec getElapsedTimespec();
};

} // namespace nix

} // namespace zge

#endif