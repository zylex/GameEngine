#ifndef GAME_H
#define GAME_H

#include <string>

#ifdef MSVC

#include "DirectXGame.h"
namespace zge
{
typedef dx::DirectXGame BaseGame;
}

#elif __linux__ || __APPLE__
// #else
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