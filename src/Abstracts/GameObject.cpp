#include "GameObject.h"

namespace zge
{
GameObject::GameObject()
    : position(0.0f, 0.0f, 0.0f),
      rotation(0.0f, 0.0f, 0.0f),
      scale(1.0f, 1.0f, 1.0f),
      origin(0.0f, 0.0f, 0.0f)
{
}

void GameObject::setPosition(const glm::vec3 position)
{
  this->position = position;
  this->dirty();
}

glm::vec3 GameObject::getPosition() const { return this->position; }

void GameObject::setRotation(const glm::vec3 rotation)
{
  this->rotation = rotation;
  this->dirty();
}

glm::vec3 GameObject::getRotation() const { return this->rotation; }

void GameObject::setScale(const glm::vec3 scale)
{
  this->scale = scale;
  this->dirty();
}

glm::vec3 GameObject::getScale() const { return this->scale; }

void GameObject::setOrigin(const glm::vec3 origin)
{
  this->origin = origin;
  this->dirty();
}

glm::vec3 GameObject::getOrigin() const { return this->origin; }

void GameObject::setMass(const float mass) { this->mass = mass; }

float GameObject::getMass() const { return this->mass; }

} // namespace zge
