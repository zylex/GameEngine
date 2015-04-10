#ifndef IGAMEOBJECT_H
#define IGAMEOBJECT_H

#include <glm/glm.hpp>

#include "Preprocessors.h"

namespace zge
{
// forward declarations
class IMesh;
class IMaterial;

class IGameObject
{
public:
  virtual ~IGameObject() NOEXCEPT = default;

  virtual void update() = 0;
  virtual void draw() = 0;

  virtual void setPosition(const glm::vec3 position) = 0;
  virtual glm::vec3 getPosition() const = 0;
  virtual void setRotation(const glm::vec3 eulerRotation) = 0;
  virtual glm::vec3 getRotation() const = 0;
  virtual void setScale(const glm::vec3 scale) = 0;
  virtual glm::vec3 getScale() const = 0;
  virtual void setOrigin(const glm::vec3 origin) = 0;
  virtual glm::vec3 getOrigin() const = 0;
};

} // namespace zge

#endif