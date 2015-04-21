#ifndef INSTANCEDSHADER_H
#define INSTANCEDSHADER_H

#include <vector>

#include "ShaderProgram.h"

#include "MeshObject.h"

namespace zge
{

template <typename I, typename D, typename F, typename S>
class InstancedShader : public ShaderProgram
{
private:
  std::vector<I> instances;

public:
  // constructor
  InstancedShader();
  // destructor
  virtual ~InstancedShader() NOEXCEPT;
  virtual void addInstance(IGameObject*);
  virtual void clear();

  virtual void prepare();
  virtual void render();

  virtual void executeShader(const I&) const = 0;
  virtual const D getInstanceData(const MeshObject*) const = 0;
  virtual const I getInstanceUnit(MeshObject*, const D&) const = 0;

  // struct InstanceFinder
  // {
  //   MeshObject* instance;
  //   InstanceFinder(MeshObject*);
  //   virtual bool operator()(const I&) const = 0;
  // };

  // struct InstanceSorter
  // {
  //   virtual bool operator()(const I&, const I&) = 0;
  // };

  void updateData(std::vector<MeshObject*>&, std::vector<D>&);
};

} // namespace zge

#endif
