#ifdef USE_ANT

#ifndef STATS_H
#define STATS_H

#include "Preprocessors.h"

#include "Timer.h"

namespace zge
{

class Stats
{
private:
  Timer mainThreadTimer;
  float mainThreadFPS;
  float mainThreadFrameTime;
  long long mainThreadFrameCounter;
  long long mainThreadTimeCounter;

  Timer networkThreadTimer;
  float networkThreadFPS;
  float networkThreadFrameTime;
  long long networkThreadFrameCounter;
  long long networkThreadTimeCounter;

  Timer simulationThreadTimer;
  float simulationThreadFPS;
  float simulationThreadFrameTime;
  long long simulationThreadFrameCounter;
  long long simulationThreadTimeCounter;

public:
  static Stats* getInstance();
  // destructor
  virtual ~Stats() NOEXCEPT;
  // copy constructor
  Stats(const Stats&);
  // assignment operator
  Stats& operator=(const Stats&);
  // move constructor (C++11)
  Stats(Stats&&) NOEXCEPT;
  // move assignment operator (C++11)
  Stats& operator=(Stats&&) NOEXCEPT;

  void clear();

  void mainThreadTick();
  void networkThreadTick();
  void simulationThreadTick();

  const float& getMainFPS();
  const float& getNetworkFPS();
  const float& getSimulationFPS();

  const float& getMainFrameTime();
  const float& getNetworkFrameTime();
  const float& getSimulationFrameTime();

  // unsigned& getNbOfVertices();
  // unsigned& getNbOfTriangles();
  // unsigned& getNbOfInstances();

  static unsigned numberOfVertices;
  static unsigned numberOfTriangles;
  static unsigned numberOfInstances;

private:
  // constructor
  Stats();
};

} // namespace zge

#endif

#endif
