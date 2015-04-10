#include "GameObject.h"

namespace zge
{
void GameObject::setPosition(const glm::vec3 position)
{
  this->position = position;
}

glm::vec3 GameObject::getPosition() const { return this->position; }

void GameObject::setRotation(const glm::vec3 rotation)
{
  this->rotation = rotation;
}

glm::vec3 GameObject::getRotation() const { return this->rotation; }

void GameObject::setScale(const glm::vec3 scale) { this->scale = scale; }

glm::vec3 GameObject::getScale() const { return this->scale; }

void GameObject::setOrigin(const glm::vec3 origin) { this->origin = origin; }

glm::vec3 GameObject::getOrigin() const { return this->origin; }

} // namespace zge