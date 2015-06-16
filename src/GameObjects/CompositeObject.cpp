#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CompositeObject.h"

namespace zge
{

CompositeObject::CompositeObject()
{
  // constructor
}

CompositeObject::~CompositeObject() noexcept
{
  // destructor
}

CompositeObject::CompositeObject(const CompositeObject& other)
{
  // copy constructor
}

CompositeObject& CompositeObject::operator=(const CompositeObject& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

CompositeObject::CompositeObject(CompositeObject&& other) noexcept
{
  // move constructor (C++11)
}

CompositeObject& CompositeObject::operator=(CompositeObject&& other) noexcept
{
  // move assignment operator (C++11)
  return *(new CompositeObject(other));
}

std::vector<IGameObject*> CompositeObject::getChildren() const
{
  return this->children;
}

void CompositeObject::setChildren(std::vector<IGameObject*> children)
{
  this->children = children;
}

void CompositeObject::addChild(IGameObject* gameObject)
{
  this->children.push_back(gameObject);
}
void CompositeObject::clear() { this->children.clear(); }

void CompositeObject::addInstance(IShaderProgram* program)
{
  std::vector<IGameObject*>::iterator end = this->children.end();
  for (std::vector<IGameObject*>::iterator it = this->children.begin();
       it IS_NOT end; ++it)
  {
    (*it)->addInstance(program);
  }
}

void CompositeObject::setPosition(const glm::vec3 position)
{
  GameObject::setPosition(position);
  std::vector<IGameObject*>::iterator end = this->children.end();
  for (std::vector<IGameObject*>::iterator it = this->children.begin();
       it IS_NOT end; ++it)
  {
    (*it)->setOrigin(position);
  }
}

void CompositeObject::setRotation(const glm::vec3 rotation)
{
  std::vector<IGameObject*>::iterator end = this->children.end();
  for (std::vector<IGameObject*>::iterator it = this->children.begin();
       it IS_NOT end; ++it)
  {
    // TODO: rotate properly around the current world position
    glm::vec3 diff = rotation - this->getRotation();
    // (*it)->setRotation(rotation);

    glm::mat4 rotationMatrix = glm::translate(-(*it)->getPosition());
    rotationMatrix *= glm::eulerAngleYXZ(
        glm::radians(diff.y), glm::radians(diff.x), glm::radians(diff.z));
    rotationMatrix = glm::translate(rotationMatrix, (*it)->getPosition());
    glm::vec4 position = glm::vec4((*it)->getPosition(), 1.0f) * rotationMatrix;
    (*it)->setPosition(glm::vec3(position.x, position.y, position.z));
    // (*it)->setRotation((*it)->getRotation() - diff);

    glm::vec3 itRotation = (*it)->getRotation();

    (*it)->setRotation(
        glm::vec3(glm::vec4(glm::radians(itRotation), 1.0f) * rotationMatrix));
  }
  GameObject::setRotation(rotation);
}

void CompositeObject::setScale(const glm::vec3 scale)
{
  GameObject::setScale(scale);
  std::vector<IGameObject*>::iterator end = this->children.end();
  for (std::vector<IGameObject*>::iterator it = this->children.begin();
       it IS_NOT end; ++it)
  {
    (*it)->setScale(scale);
    glm::vec3 position = (*it)->getPosition();
    position *= scale;
    (*it)->setPosition(position);
  }
}

void CompositeObject::setOrigin(const glm::vec3 origin)
{
  GameObject::setOrigin(origin);
  std::vector<IGameObject*>::iterator end = this->children.end();
  for (std::vector<IGameObject*>::iterator it = this->children.begin();
       it IS_NOT end; ++it)
  {
    (*it)->setOrigin(origin + this->getPosition());
  }
}

void CompositeObject::update(double deltaTime)
{
  // std::vector<IGameObject*>::iterator end = this->children.end();
  // for (std::vector<IGameObject*>::iterator it = this->children.begin();
  //      it IS_NOT end; ++it)
  // {
  //   (*it)->update();
  // }
}

void CompositeObject::draw()
{
  // std::vector<IGameObject*>::iterator end = this->children.end();
  // for (std::vector<IGameObject*>::iterator it = this->children.begin();
  //      it IS_NOT end; ++it)
  // {
  //   (*it)->draw();
  // }
}

void CompositeObject::clean()
{
  Dirty::clean();
  std::vector<IGameObject*>::iterator end = this->children.end();
  for (std::vector<IGameObject*>::iterator it = this->children.begin();
       it IS_NOT end; ++it)
  {
    (*it)->clean();
  }
}

} // namespace zge
