#ifndef GAME_H
#define GAME_H

#include <string>

#ifdef DIRECT_X

#include "DirectXGame.h"
namespace zge
{
typedef dx::DirectXGame BaseGame;
}

// #elif OPEN_GL
#else
#include "OpenGLGame.h"
namespace zge
{
typedef gl::OpenGLGame BaseGame;
}

#endif

namespace zge
{
class Game : public BaseGame
{
public:
  Game(std::string);
  virtual ~Game() noexcept;

private:
};

} // namespace zge

#endif