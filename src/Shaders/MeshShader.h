#ifndef MESHSHADER_H
#define MESHSHADER_H

#include <glm/mat4x4.hpp>

#include "InstancedShader.h"
#include "InstancedShader.cpp"

#include "MeshObject.h"

namespace zge
{

namespace MeshShaderStructs
{

// struct Instance;
struct Instance
{
  unsigned meshId;
  std::vector<glm::mat4> instanceData;
  std::vector<MeshObject*> instanceObjects;
};

struct InstanceFinder
{
  MeshObject* instance;
  InstanceFinder(MeshObject*);
  bool operator()(const Instance&) const;
};

struct InstanceSorter
{
  bool operator()(const Instance&, const Instance&);
};

} // namespace MeshShaderStructs

using namespace MeshShaderStructs;

class MeshShader : public InstancedShader<Instance, glm::mat4, InstanceFinder,
                                          InstanceSorter>
{
private:
public:
  // constructor
  MeshShader();
  // destructor
  virtual ~MeshShader() NOEXCEPT;

private:
  void executeShader(const Instance&) const;
  const glm::mat4 getInstanceData(const MeshObject*) const;
  const Instance getInstanceUnit(MeshObject*, const glm::mat4&) const;
};

} // namespace zge

#endif
