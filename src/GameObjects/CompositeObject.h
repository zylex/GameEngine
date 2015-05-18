#ifndef COMPOSITEOBJECT_H
#define COMPOSITEOBJECT_H

#include <vector>

#include "Preprocessors.h"

#include "IGameObject.h"
#include "GameObject.h"

namespace zge
{

class CompositeObject : public GameObject
{
private:
  std::vector<IGameObject*> children;

public:
  // constructor
  CompositeObject();
  // destructor
  virtual ~CompositeObject() NOEXCEPT;
  // copy constructor
  CompositeObject(const CompositeObject&);
  // assignment operator
  CompositeObject& operator=(const CompositeObject&);
  // move constructor (C++11)
  CompositeObject(CompositeObject&&) NOEXCEPT;
  // move assignment operator (C++11)
  CompositeObject& operator=(CompositeObject&&) NOEXCEPT;

  void update();
  void draw();

  void addInstance(IShaderProgram*);

  std::vector<IGameObject*> getChildren() const;
  void setChildren(std::vector<IGameObject*>);
  void addChild(IGameObject*);
  void clear();

  void setPosition(const glm::vec3);
  void setRotation(const glm::vec3);
  void setScale(const glm::vec3);
  void setOrigin(const glm::vec3);

  void clean();
};

} // namespace zge

#endif
