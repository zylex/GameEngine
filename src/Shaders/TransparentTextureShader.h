#ifndef TRANSPARENTTEXTURESHADER_H
#define TRANSPARENTTEXTURESHADER_H

#include <vector>

#include <glm/mat4x4.hpp>

#include "ShaderProgram.h"

#include "MeshObject.h"

namespace zge
{

class TransparentTextureShader : public ShaderProgram
{
private:
  struct Instance
  {
    unsigned textureId;
    unsigned meshId;
    std::vector<glm::mat4> instanceData;
    std::vector<MeshObject*> instanceObjects;
  };

  struct InstanceFinder
  {
    MeshObject* object;
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

  struct LightData
  {
    glm::vec4 ambientColor;
    glm::vec4 diffuseColor;
    glm::vec4 specularColor;
    glm::vec3 vector; // position if point light, direction if directional
    float specularPower;
    glm::vec3 viewPosition;
    int lightType; // 0 for directional, 1 for point, 2 for other
  };

private:
  std::vector<Instance> instances;
  LightData lightData;

public:
  // constructor
  TransparentTextureShader();
  // destructor
  virtual ~TransparentTextureShader() NOEXCEPT;

  const bool initialise();
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
