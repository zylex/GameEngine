#include "IShaderProgram.h"

#include "DepthShader.h"
#include "PsychedaelicShader.h"

#include "Shader.h"

#include "ShaderManager.h"

namespace zge
{

IShaderManager* IShaderManager::getInstance()
{
  return ShaderManager::getInstance();
}

ShaderManager* ShaderManager::getInstance()
{
  static ShaderManager instance;
  return &instance;
}

ShaderManager::ShaderManager() : shaders(SHADER_TOTAL)
{
  // constructor
}

ShaderManager::~ShaderManager() NOEXCEPT
{
  for (std::vector<IShaderProgram*>::iterator i = this->shaders.begin();
       i IS_NOT this->shaders.end(); ++i)
  {
    SAFE_DELETE((*i));
  }
}

const bool ShaderManager::initialise()
{

  for (int i = SHADER_NULL + 1; i < SHADER_TOTAL; ++i)
  {
    this->shaders[i] = this->createShader(i);

    if (not this->shaders[i]->initialise())
    {
      return false;
    }
  }

  return true;
}

void ShaderManager::addInstance(unsigned shaderId, IGameObject* gameObject)
{
  this->shaders[shaderId]->addInstance(gameObject);
}

void ShaderManager::executeShaders()
{
  std::vector<IShaderProgram*>::iterator it, end = this->shaders.end();
  for (it = this->shaders.begin() + 1; it IS_NOT end; ++it)
  {
    (*it)->prepare();
    (*it)->render();
    (*it)->finish();
  }
}

const unsigned ShaderManager::addShader(IShaderProgram* shader)
{
  this->shaders.push_back(shader);
  return this->shaders.size() - 1;
}

IShaderProgram* ShaderManager::getShader(const unsigned shaderType)
{
  return this->shaders[shaderType];
}

IShaderProgram* ShaderManager::createShader(const unsigned shaderType)
{
  switch (shaderType)
  {
    case SHADER_DEPTH:
    {
      return new DepthShader();
    }
    case SHADER_PSYCHEDAELIC:
    {
      return new PsychedaelicShader(
          (dynamic_cast<DepthShader*>(IShaderManager::getInstance()->getShader(
               SHADER_DEPTH)))->getDepthTexture());
    }
    default:
    {
      return nullptr;
    }
  }
}

} // namespace zge
