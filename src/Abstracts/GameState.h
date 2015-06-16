#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <vector>

#include "IGameState.h"
#include "IGameObject.h"
#include "Camera.h"
#include "Light.h"

namespace zge
{

class GameState : public IGameState
{
private:
  Camera* currentCamera;

  std::vector<IGameObject*> gameObjects;
  std::vector<Light*> lights;

public:
  GameState();
  virtual ~GameState() NOEXCEPT;

  void update(double deltaTime);
  void draw();

  void setCurrentCamera(Camera* camera);
  Camera* getCurrentCamera() const;
  void addGameObject(unsigned, IGameObject*);

  void cleanAll();

  void addLight(Light*);
  std::vector<Light*> getLights() const;
};

} // namespace zge

#endif
