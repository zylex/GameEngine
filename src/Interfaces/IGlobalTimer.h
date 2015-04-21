#ifndef IGLOBALTIMER_H
#define IGLOBALTIMER_H

// #define TIMER_RESOLUTION 1000000000LL

namespace zge
{

class IGlobalTimer
{
public:
  //                                        1000ooo000
  static const long long TIMER_RESOLUTION = 1000000000LL;

public:
  static IGlobalTimer* getInstance();
  // elapsed time since start of program
  virtual const long double getElapsedSeconds() = 0;
  virtual const long long getElapsedNanoSeconds() = 0;
};

} // namespace zge

#endif