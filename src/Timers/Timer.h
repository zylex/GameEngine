#ifndef TIMER_H
#define TIMER_H

#include "Preprocessors.h"

namespace zge
{
// forward declarations
class IGlobalTimer;

class Timer
{
private:
  IGlobalTimer* globaltimer;
  long long startTime;
  long long endTime;

public:
  // constructor
  Timer();
  // destructor
  virtual ~Timer() NOEXCEPT;
  // copy constructor
  Timer(const Timer&);
  // assignment operator
  Timer& operator=(const Timer&);
  // move constructor (C++11)
  Timer(Timer&&) NOEXCEPT;
  // move assignment operator (C++11)
  Timer& operator=(Timer&&) NOEXCEPT;

  // void start(); // start a construction
  void stop();

  // returns the elapsed time
  long long reset();
  // since start (in nanoseconds)
  long long getElapsedTime();
  // start time - end time (from using stop, also in nanoseconds)
  long long getDeltaTime();
};

} // namespace zge

#endif
