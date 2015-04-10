#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <vector>
#include <unordered_map>
#include <glm/mat4x4.hpp>

#include "IShaderProgram.h"

#include "MeshObject.h"

namespace zge
{

class ShaderProgram : public IShaderProgram
{
private:
  unsigned programId;
  unsigned outputId;
  unsigned depthState;

public:
  ShaderProgram();
  virtual ~ShaderProgram() NOEXCEPT = default;

  virtual void prepare();
  virtual void finish();

  void setProgramId(const unsigned);
  const unsigned getProgramId() const;

  void setOutputId(const unsigned);
  const unsigned getOutputId() const;

  void setDepthState(const unsigned);
  const unsigned getDepthState() const;
};

} // namespace zge

#endif