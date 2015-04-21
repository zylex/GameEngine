#ifndef ISHADERMANAGER_H
#define ISHADERMANAGER_H

#include "Preprocessors.h"

namespace zge
{
// forward declarations
class IGameObject;
class IShaderProgram;

class IShaderManager
{
public:
  static IShaderManager* getInstance();
  virtual ~IShaderManager() NOEXCEPT = default;

  virtual const bool initialise() = 0;
  virtual void executeShaders() = 0;

  virtual void addInstance(const unsigned shaderId,
                           IGameObject* gameObject) = 0;

  virtual const unsigned addShader(IShaderProgram* shader) = 0;
  virtual IShaderProgram* getShader(const unsigned shaderType) = 0;
};

} // namespace zge

#endif
