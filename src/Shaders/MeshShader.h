#ifndef MESHSHADER_H
#define MESHSHADER_H

#include <vector>
// #include <unordered_map>

#include <glm/mat4x4.hpp>

// #include "InstancedShader.h"
// #include "InstancedShader.cpp"
#include "ShaderProgram.h"

#include "MeshObject.h"

namespace zge
{

class MeshShader : public ShaderProgram
{
private:
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
    const bool operator()(const Instance&) const;
  };

  struct ObjectFinder
  {
    MeshObject* instance;
    ObjectFinder(MeshObject*);
    const bool operator()(const MeshObject*) const;
  };

  struct InstanceSorter
  {
    const bool operator()(const Instance&, const Instance&);
  };

private:
  std::vector<Instance> instances;

public:
  // constructor
  MeshShader();
  // destructor
  virtual ~MeshShader() NOEXCEPT;

  void clear();
  void prepare();
  void render();

  virtual void addInstance(IGameObject*);
  virtual void addInstance(MeshObject*);

private:
  void executeShader(const Instance&) const;
  const glm::mat4 getInstanceData(const MeshObject*) const;
  const Instance createInstanceUnit(MeshObject*, const glm::mat4&) const;

  void updateData(std::vector<MeshObject*>&, std::vector<glm::mat4>&);
};

} // namespace zge

#endif
