#ifndef DIRECTXRESOURCEMANAGER_H
#define DIRECTXRESOURCEMANAGER_H

#include <d3d11.h>
#include <vector>
#include <pair>
#include <glm/glm.hpp>

#include "ResourceManager.h"

namespace zge
{
namespace dx
{

class DirectXResourceManager : ResourceManager
{
private:
  std::vector<ID3D11Buffer*> _buffers;
  std::vector<std::pair<unsigned, unsigned> > _meshes;

public:
  static DirectXResourceManager* getInstance();
  // destructor
  virtual ~DirectXResourceManager() NOEXCEPT;
  // copy constructor
  DirectXResourceManager(const DirectXResourceManager&);
  // assignment operator
  DirectXResourceManager& operator=(const DirectXResourceManager&);
  // move constructor (C++11)
  DirectXResourceManager(DirectXResourceManager&&) NOEXCEPT;
  // move assignment operator (C++11)
  DirectXResourceManager& operator=(DirectXResourceManager&&) NOEXCEPT;

  const unsigned int createMesh(const std::vector<glm::vec3>,
                                const std::vector<unsigned int>) const;
  const unsigned int createInstanceBuffer(const std::vector<glm::mat4>) const;

  const unsigned int createDepthBuffer(const unsigned int) const;

private:
  // constructor
  DirectXResourceManager();
};

} // namespace dx

} // namespace zge

#endif