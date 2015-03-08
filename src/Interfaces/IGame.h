#ifndef IGAME_H
#define IGAME_H

namespace zge
{

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const int WINDOWED_WIDTH = 800;
const int WINDOWED_HEIGHT = 600;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float SHADOWMAP_DEPTH = 50.0f;
const float SHADOWMAP_NEAR = 1.0f;

const int SCREEN_WIDTH(const int = 0);
const int SCREEN_HEIGHT(const int = 0);

// class IGameState;
// class IRenderer;

class IGame
{
public:
  virtual bool initialise() = 0;
  virtual void run() = 0;
  virtual void quit() = 0;

  // virtual void addGameState(int, IGameState*) = 0;
  virtual void setNextState(int) = 0;
  // virtual IRenderer* getRenderer() = 0;
};

} // namespace zge

#endif