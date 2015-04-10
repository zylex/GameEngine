#include "IRenderer.h"

#include "GameState.h"

namespace zge
{
GameState::GameState() : _currentCamera(nullptr) {}

GameState::~GameState() NOEXCEPT
{
  SAFE_DELETE(_currentCamera);
  for (unsigned i = 0; i < _gameObjects.size(); ++i)
  {
    SAFE_DELETE(_gameObjects[i]);
  }
}

// MAYBE: Move this to separate simulation thread
void GameState::update()
{

  for (std::vector<IGameObject*>::iterator it = _gameObjects.begin();
       it != _gameObjects.end(); ++it)
  {
    (*it)->update();
  }
}

void GameState::setCurrentCamera(Camera* camera) { _currentCamera = camera; }

Camera* GameState::getCurrentCamera() const { return _currentCamera; }

void GameState::addGameObject(unsigned shaderId, IGameObject* gameObject)
{
  _gameObjects.push_back(gameObject);

  IRenderer::getInstance()->addInstance(shaderId, gameObject);
}

} // namespace zge