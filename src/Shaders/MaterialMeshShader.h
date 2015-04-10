#ifndef MATERIALMESHSHADER_H
#define MATERIALMESHSHADER_H

#include <glm/mat4x4.hpp>

#include "InstancedShader.h"

#include "MeshObject.h"

namespace zge
{
namespace MaterialMeshShaderStructs
{

struct InstanceData
{
  glm::mat4 instanceMatrix;
  int shininess;
};

struct Instance
{
  unsigned materialId;
  unsigned meshId;
  std::vector<InstanceData> instanceData;
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

} // namespace MaterialMeshShaderStructs

using namespace MaterialMeshShaderStructs;

class MaterialMeshShader
    : public InstancedShader<Instance, InstanceData, InstanceFinder,
                             InstanceSorter>
{
private:
public:
  // constructor
  MaterialMeshShader();
  // destructor
  virtual ~MaterialMeshShader() NOEXCEPT;

private:
  void executeShader(const Instance&) const;
  const InstanceData getInstanceData(const MeshObject*) const;
  const Instance getInstanceUnit(MeshObject*, const InstanceData&) const;
};

} // namespace zge

#endif