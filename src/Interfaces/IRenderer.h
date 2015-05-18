#ifndef IRENDERER_H
#define IRENDERER_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Preprocessors.h"

namespace zge
{
// forward declarations
class IShaderProgram;
class IGameObject;

class IRenderer
{
public:
  // MAYBE: set viewport?

  virtual ~IRenderer() NOEXCEPT = default;

  static IRenderer* getInstance();
  virtual const bool initialise() = 0;

  virtual void setShaderProgram(const unsigned programId) = 0;

  virtual void setConstants(
      const void** bufferDataArray,
      const std::vector<unsigned long> bufferDataByteSizes,
      const unsigned shaderType) = 0;
  virtual void setConstant(const unsigned indexPosition, const void* bufferData,
                           const unsigned long bufferDataByteSize,
                           const unsigned shaderType) = 0;

  virtual void executeInstancedShader(
      const unsigned meshId, const void* instanceDataArray,
      const unsigned long instanceDataUnitByteSize,
      const unsigned numberOfInstances) = 0;
  virtual void executeComputeShader() = 0;
  virtual void executeShader() = 0;

  virtual void setTextures(const std::vector<unsigned> textureIds,
                           const std::vector<unsigned> samplerTypes,
                           const unsigned shaderType) = 0;
  virtual void setTexture(const unsigned textureLocation,
                          const unsigned textureId, const unsigned samplerType,
                          const unsigned shaderType) = 0;

  virtual void setOutput(const unsigned outputId) = 0;

  // TODO: implement state changing methods (depth/culling etc)
  virtual void enableDepth() = 0;
  // virtual void enableReadDepth() = 0;
  // virtual void enableWriteDepth() = 0;
  virtual void disableDepth() = 0;
  virtual void enableAlphaBlending() = 0;
  virtual void disableAlphaBlending() = 0;
  virtual void enableAntiClockwiseCulling() = 0;
  virtual void disableCulling() = 0;

  // implemented by abstract class:
  virtual void addInstance(const unsigned shaderId,
                           IGameObject* gameObject) = 0;
  virtual void render() = 0;
  virtual void swapBuffers() = 0;

#ifdef USE_ANT
  virtual void toggleAntTweakBar() = 0;
#endif
};

} // namespace zge

#endif
