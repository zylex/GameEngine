#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <vector>

#include "IGameState.h"
#include "IGameObject.h"
#include "Camera.h"

namespace zge
{

class GameState : public IGameState
{
private:
  Camera* currentCamera;

  std::vector<IGameObject*> gameObjects;

public:
  GameState();
  virtual ~GameState() NOEXCEPT;

  void update();
  void draw();

  void setCurrentCamera(Camera* camera);
  Camera* getCurrentCamera() const;
  void addGameObject(unsigned, IGameObject*);

  void cleanAll();
};

} // namespace zge

#endif
