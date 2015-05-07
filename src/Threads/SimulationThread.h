#ifndef SIMULATIONTHREAD_H
#define SIMULATIONTHREAD_H

#include "AffinityThread.h"

namespace zge
{

class SimulationThread : public AffinityThread
{
private:
  static unsigned core_number;

public:
  SimulationThread();
  virtual ~SimulationThread() NOEXCEPT;

  virtual void callback() = 0;
};

} // namespace zge

#endif
