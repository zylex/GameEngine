#ifndef ISHADERPROGRAM_H
#define ISHADERPROGRAM_H

#include "Preprocessors.h"

namespace zge
{
// forward declarations
class IGameObject;
class MeshObject;

class IShaderProgram
{
public:
  virtual ~IShaderProgram() NOEXCEPT = default;

  virtual void addInstance(IGameObject* instance) = 0;
  virtual void addInstance(MeshObject* instance) = 0;
  virtual void clear() = 0;

  virtual const bool initialise() = 0;
  virtual void prepare() = 0;
  virtual void render() = 0;
  virtual void finish() = 0;

  virtual const unsigned getProgramId() const = 0;
};

} // namespace zge

#endif
