#ifndef SIMULATIONTHREAD_H
#define SIMULATIONTHREAD_H

#include "AffinityThread.h"

namespace zge
{

class SimulationThread : public AffinityThread
{
private:
  static const unsigned CORE_NUMBER;

public:
  SimulationThread();
  virtual ~SimulationThread() NOEXCEPT;

  virtual void callback() = 0;
};

} // namespace zge

#endif
