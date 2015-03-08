#ifndef ABSTRACT_GAME_H
#define ABSTRACT_GAME_H

#include "IGame.h"

namespace zge
{

class AbstractGame
{
public:
  virtual bool initialise() = 0;
  virtual void run() = 0;
  virtual void quit() = 0;

  virtual void setNextState(int) = 0;
};

} // namespace zge

#endif