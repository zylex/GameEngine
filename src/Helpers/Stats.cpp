#include "Stats.h"

namespace zge
{

unsigned Stats::numberOfVertices = 0.0f;
unsigned Stats::numberOfTriangles = 0.0f;
unsigned Stats::numberOfInstances = 0.0f;

Stats* Stats::getInstance()
{
  static Stats instance;
  return &instance;
}

Stats::Stats()
    : mainThreadFPS(0.0f), networkThreadFPS(0.0f), simulationThreadFPS(0.0f)
{

  // constructor
}

Stats::~Stats() NOEXCEPT
{
  // destructor
}

Stats::Stats(const Stats& other)
{
  // copy constructor
}

Stats& Stats::operator=(const Stats& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

Stats::Stats(Stats&& other) NOEXCEPT
{
  // move constructor (C++11)
}

Stats& Stats::operator=(Stats&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new Stats(other));
}

void Stats::clear()
{
  this->mainThreadFPS = 0.0f;
  this->networkThreadFPS = 0.0f;
  this->simulationThreadFPS = 0.0f;

  zge::Stats::numberOfVertices = 0;
  zge::Stats::numberOfTriangles = 0;
  zge::Stats::numberOfInstances = 0;
}

void Stats::mainThreadTick()
{
  this->mainThreadFPS = 1e+9 / this->mainThreadTimer.reset();
  // this->visualizationThreadFPS =
  //     1e+9 / this->visualizationThreadTimer.getElapsedTime();
  // this->networkThreadFPS = 1e+9 / this->networkThreadTimer.getElapsedTime();
  // this->simulationThreadFPS =
  //     1e+9 / this->simulationThreadTimer.getElapsedTime();
}

void Stats::networkThreadTick()
{
  this->networkThreadFPS = 1e+9 / this->networkThreadTimer.reset();
  // this->mainThreadFPS = 1e+9 / this->mainThreadTimer.getElapsedTime();
  // this->visualizationThreadFPS =
  //     1e+9 / this->visualizationThreadTimer.getElapsedTime();
  // this->simulationThreadFPS =
  //     1e+9 / this->simulationThreadTimer.getElapsedTime();
}

void Stats::simulationThreadTick()
{
  this->simulationThreadFPS = 1e+9 / this->simulationThreadTimer.reset();
  // this->mainThreadFPS = 1e+9 / this->mainThreadTimer.getElapsedTime();
  // this->visualizationThreadFPS =
  //     1e+9 / this->visualizationThreadTimer.getElapsedTime();
  // this->networkThreadFPS = 1e+9 / this->networkThreadTimer.getElapsedTime();
}

const float& Stats::getMainFPS() { return this->mainThreadFPS; }

const float& Stats::getNetworkFPS() { return this->networkThreadFPS; }

const float& Stats::getSimulationFPS() { return this->simulationThreadFPS; }

} // namespace zge
