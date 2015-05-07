#ifndef ABSTRACT_GAME_H
#define ABSTRACT_GAME_H
#include <unordered_map>
#include <string>

#include "IGame.h"

namespace zge
{
class IGameState;

class Game : public IGame
{
private:
  bool running;
  int nextState;
  std::unordered_map<int, IGameState*> states;
  IGameState* currentState;
  std::string windowTitle;

#ifdef USE_ANT
  bool showAntTweakBar = false;
  bool twNotTerminated = true;
#endif

public:
  Game();
  virtual ~Game() NOEXCEPT;

  virtual const bool initialise();
  virtual const int run();
  virtual void frame();
  virtual void shutdown();

  void setWindowTitle(const std::string);
  const std::string& getWindowTitle() const;

  void quit();
  void addGameState(const int, IGameState*);
  void setNextState(const int);
  IGameState* getCurrentGameState() const;

  void changeState();
  const bool isRunning() const;

  const std::unordered_map<int, IGameState*> getStates();

  static const unsigned MAIN_THREAD_AFFINITY = 0;
};

} // namespace zge

#endif
