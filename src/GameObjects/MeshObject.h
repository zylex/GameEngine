#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include "IMaterial.h"

#include "GameObject.h"

namespace zge
{

class MeshObject : public GameObject
{
private:
  unsigned meshId;
  IMaterial* material;

public:
  // constructor
  MeshObject();
  // destructor
  virtual ~MeshObject() NOEXCEPT;
  // copy constructor
  MeshObject(const MeshObject&);
  // assignment operator
  MeshObject& operator=(const MeshObject&);
  // move constructor (C++11)
  MeshObject(MeshObject&&) NOEXCEPT;
  // move assignment operator (C++11)
  MeshObject& operator=(MeshObject&&) NOEXCEPT;

  void update();
  void draw();

  void setMeshId(const unsigned);
  unsigned getMeshId() const;
  void setMaterial(IMaterial*);
  IMaterial* getMaterial() const;

  void addInstance(IShaderProgram*);
};

} // namespace zge

#endif
