#include "IRenderer.h"

#include "GameState.h"

namespace zge
{
GameState::GameState() : currentCamera(nullptr) {}

GameState::~GameState() NOEXCEPT
{
  SAFE_DELETE(this->currentCamera);
  for (unsigned i = 0; i < this->gameObjects.size(); ++i)
  {
    SAFE_DELETE(this->gameObjects[i]);
  }
}

// MAYBE: Move this to separate simulation thread
void GameState::update()
{

  for (std::vector<IGameObject*>::iterator it = this->gameObjects.begin();
       it != this->gameObjects.end(); ++it)
  {
    (*it)->update();
  }
}

void GameState::setCurrentCamera(Camera* camera)
{
  this->currentCamera = camera;
}

Camera* GameState::getCurrentCamera() const { return this->currentCamera; }

void GameState::addGameObject(unsigned shaderId, IGameObject* gameObject)
{
  this->gameObjects.push_back(gameObject);

  IRenderer::getInstance()->addInstance(shaderId, gameObject);
}

void GameState::cleanAll()
{
  std::vector<IGameObject*>::iterator it, end = this->gameObjects.end();
  for (it = this->gameObjects.begin(); it IS_NOT end; ++it)
  {
    (*it)->clean();
  }
}

} // namespace zge
