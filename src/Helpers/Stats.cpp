#include "Stats.h"
#include <iostream>

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
    : mainThreadTimer(),
      mainThreadFPS(0.0f),
      mainThreadFrameTime(0.0f),
      mainThreadFrameCounter(0),
      mainThreadTimeCounter(0),
      networkThreadTimer(),
      networkThreadFPS(0.0f),
      networkThreadFrameTime(0.0f),
      networkThreadFrameCounter(0),
      networkThreadTimeCounter(0),
      simulationThreadTimer(),
      simulationThreadFPS(0.0f),
      simulationThreadFrameTime(0.0f),
      simulationThreadFrameCounter(0),
      simulationThreadTimeCounter(0)
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

  this->mainThreadFrameCounter = 0;
  this->networkThreadFrameCounter = 0;
  this->simulationThreadFrameCounter = 0;
}

void Stats::mainThreadTick()
{
  ++this->mainThreadFrameCounter;
  long long elapsed = this->mainThreadTimer.reset();
  this->mainThreadFrameTime = static_cast<float>(elapsed / 1e+6);
  this->mainThreadTimeCounter += elapsed;
  if (this->mainThreadTimeCounter > 1e+9)
  {
    this->mainThreadFPS =
        this->mainThreadFrameCounter * 1e+9 / this->mainThreadTimeCounter;
    this->mainThreadTimeCounter -= 1e+9;
    this->mainThreadFrameCounter = 0;
  }

  // this->mainThreadFPS = 1e+9 / this->mainThreadTimer.reset();

  // this->visualizationThreadFPS =
  //     1e+9 / this->visualizationThreadTimer.getElapsedTime();
  // this->networkThreadFPS = 1e+9 / this->networkThreadTimer.getElapsedTime();
  // this->simulationThreadFPS =
  //     1e+9 / this->simulationThreadTimer.getElapsedTime();
}

void Stats::networkThreadTick()
{
  ++this->networkThreadFrameCounter;
  long long elapsed = this->networkThreadTimer.reset();
  this->networkThreadFrameTime = static_cast<float>(elapsed / 1e+6);
  this->networkThreadTimeCounter += elapsed;
  if (this->networkThreadTimeCounter > 1e+9)
  {
    this->networkThreadFPS =
        this->networkThreadFrameCounter * 1e+9 / this->networkThreadTimeCounter;
    this->networkThreadTimeCounter -= 1e+9;
    this->networkThreadFrameCounter = 0;
  }

  // this->networkThreadFPS = 1e+9 / this->networkThreadTimer.reset();
  // this->mainThreadFPS = 1e+9 / this->mainThreadTimer.getElapsedTime();
  // this->visualizationThreadFPS =
  //     1e+9 / this->visualizationThreadTimer.getElapsedTime();
  // this->simulationThreadFPS =
  //     1e+9 / this->simulationThreadTimer.getElapsedTime();
}

void Stats::simulationThreadTick()
{
  ++this->simulationThreadFrameCounter;
  long long elapsed = this->simulationThreadTimer.reset();
  this->simulationThreadFrameTime = static_cast<float>(elapsed / 1e+6);
  this->simulationThreadTimeCounter += elapsed;
  if (this->simulationThreadTimeCounter > 1e+9)
  {
    this->simulationThreadFPS = this->simulationThreadFrameCounter * 1e+9 /
                                this->simulationThreadTimeCounter;
    this->simulationThreadTimeCounter -= 1e+9;
    this->simulationThreadFrameCounter = 0;
  }

  // this->simulationThreadFPS = 1e+9 / this->simulationThreadTimer.reset();
  // this->mainThreadFPS = 1e+9 / this->mainThreadTimer.getElapsedTime();
  // this->visualizationThreadFPS =
  //     1e+9 / this->visualizationThreadTimer.getElapsedTime();
  // this->networkThreadFPS = 1e+9 / this->networkThreadTimer.getElapsedTime();
}

const float& Stats::getMainFPS() { return this->mainThreadFPS; }

const float& Stats::getMainFrameTime() { return this->mainThreadFrameTime; }

const float& Stats::getNetworkFPS() { return this->networkThreadFPS; }

const float& Stats::getNetworkFrameTime()
{
  return this->networkThreadFrameTime;
}

const float& Stats::getSimulationFrameTime()
{
  return this->simulationThreadFrameTime;
}

const float& Stats::getSimulationFPS() { return this->simulationThreadFPS; }

} // namespace zge
