#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <vector>

#include "Preprocessors.h"

#include "IShaderManager.h"

namespace zge
{

class ShaderManager : public IShaderManager
{
private:
  std::vector<IShaderProgram*> shaders;

public:
  static ShaderManager* getInstance();
  // destructor
  virtual ~ShaderManager() NOEXCEPT;
  // copy constructor
  ShaderManager(const ShaderManager&);
  // assignment operator
  ShaderManager& operator=(const ShaderManager&);
  // move constructor (C++11)
  ShaderManager(ShaderManager&&) NOEXCEPT;
  // move assignment operator (C++11)
  ShaderManager& operator=(ShaderManager&&) NOEXCEPT;

  const bool initialise();
  void executeShaders();

  void addInstance(const unsigned shaderId, IGameObject* gameObject);

  const unsigned addShader(IShaderProgram* shader);
  IShaderProgram* getShader(const unsigned shaderType);

private:
  // constructor
  ShaderManager();

  IShaderProgram* createShader(const unsigned shaderType);
};

} // namespace zge

#endif
