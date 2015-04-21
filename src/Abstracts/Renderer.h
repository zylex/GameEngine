#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "IRenderer.h"

namespace zge
{

class Renderer : public IRenderer
{
private:
  unsigned currentShaderProgramId;

  unsigned depthState;
  unsigned rasterState;

public:
  Renderer() = default;
  virtual ~Renderer() NOEXCEPT;

  virtual const bool initialise();
  void render();
  void addInstance(unsigned, IGameObject*);

  virtual void setShaderProgram(const unsigned);
  const unsigned getShaderProgramId() const;
  virtual void setDepthState(const unsigned);
  const unsigned getDepthState() const;
  virtual void setRasterState(const unsigned);
  const unsigned getRasterState() const;
};

} // namespace zge

#endif
