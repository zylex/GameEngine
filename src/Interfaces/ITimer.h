#ifndef ITIMER_H
#define ITIMER_H

namespace zge
{

class ITimer
{
public:
  virtual ~ITimer() = default;
  // void start(); // start a construction
  virtual void stop() = 0;

  // returns the elapsed time
  virtual long long reset() = 0;
  // since start (in nanoseconds)
  virtual long long getElapsedTime() = 0;
  // start time - end time (from using stop, also in nanoseconds)
  virtual long long getDeltaTime() = 0;
};

} // namespace zge

#endif
