#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "IGameObject.h"

namespace zge
{

class GameObject : public IGameObject
{
private:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  glm::vec3 origin;

public:
  virtual ~GameObject() NOEXCEPT = default;

  void setPosition(const glm::vec3);
  glm::vec3 getPosition() const;
  void setRotation(const glm::vec3);
  glm::vec3 getRotation() const;
  void setScale(const glm::vec3);
  glm::vec3 getScale() const;
  void setOrigin(const glm::vec3);
  glm::vec3 getOrigin() const;
};

} // namespace zge

#endif