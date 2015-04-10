#ifndef IGAME_H
#define IGAME_H

#include <string>
#include <glm/vec4.hpp>

#include "Preprocessors.h"

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
const glm::vec4 CLEAR_COLOR = { 0.0f, 0.0f, 0.0f, 1.0f };

const int SCREEN_WIDTH(const int = 0);
const int SCREEN_HEIGHT(const int = 0);

// forward declarations
class IGameState;

class IGame
{
public:
  virtual ~IGame() NOEXCEPT = default;
  static IGame* getInstance();

  virtual const bool initialise() = 0;

  virtual void setWindowTitle(const std::string windowTitle) = 0;

  virtual const int run() = 0;

  virtual void quit() = 0;
  virtual void addGameState(const int gameStateId, IGameState* gameState) = 0;
  virtual void setNextState(const int gameStateId) = 0;
  virtual IGameState* getCurrentGameState() const = 0;
};

} // namespace zge

#endif