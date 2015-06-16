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
  long long mainThreadFrameCounter;
  long long mainThreadTimeRemainder;

  Timer networkThreadTimer;
  float networkThreadFPS;
  long long networkThreadFrameCounter;
  long long networkThreadTimeRemainder;

  Timer simulationThreadTimer;
  float simulationThreadFPS;
  long long simulationThreadFrameCounter;
  long long simulationThreadTimeRemainder;

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
