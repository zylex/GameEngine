#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "IRenderer.h"

namespace zge
{

class Renderer : public IRenderer
{
private:
  unsigned depthState;

public:
  Renderer() = default;
  virtual ~Renderer() NOEXCEPT;

  virtual const bool initialise();
  void render();
  void addInstance(unsigned, IGameObject*);

  const unsigned getDepthState() const;
  void setDepthState(const unsigned);
};

} // namespace zge

#endif