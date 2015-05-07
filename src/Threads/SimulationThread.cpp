#include "SimulationThread.h"

namespace zge
{

unsigned SimulationThread::core_number = 3;

SimulationThread::SimulationThread()
    : AffinityThread(SimulationThread::core_number)
{
  ++SimulationThread::core_number;
  if (SimulationThread::core_number > 7)
  {
    SimulationThread::core_number = 3;
  }
}

SimulationThread::~SimulationThread() NOEXCEPT { this->join(); }

} // namespace zge
