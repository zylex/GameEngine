#include "SimulationThread.h"

namespace zge
{

const unsigned SimulationThread::CORE_NUMBER = 3;

SimulationThread::SimulationThread()
    : AffinityThread(SimulationThread::CORE_NUMBER)
{
  //
}

SimulationThread::~SimulationThread() NOEXCEPT { this->join(); }

} // namespace zge

/*
worker pool for small interpolation calculations (using all cores)
*/
