#include "IRenderer.h"
#include "DepthState.h"

#include "ShaderProgram.h"

namespace zge
{

ShaderProgram::ShaderProgram()
    : programId(0), outputId(0), depthState(ZGE_DEPTH_OFF)
{
}

void ShaderProgram::setProgramId(const unsigned programId)
{
  this->programId = programId;
}

const unsigned ShaderProgram::getProgramId() const { return this->programId; }

void ShaderProgram::prepare()
{
  IRenderer* renderer = IRenderer::getInstance();
  renderer->setShaderProgram(this->programId);
  renderer->setOutput(this->outputId);
  switch (this->depthState)
  {
    case ZGE_DEPTH_OFF:
    {
      renderer->disableDepth();
      break;
    }
    case ZGE_DEPTH_ON:
    {
      renderer->enableDepth();
      break;
    }
  }
}

void ShaderProgram::finish()
{
  // TODO: implement finish or remove it
}

void ShaderProgram::setOutputId(const unsigned outputId)
{
  this->outputId = outputId;
}

const unsigned ShaderProgram::getOutputId() const { return this->outputId; }

void ShaderProgram::setDepthState(const unsigned depthState)
{
  this->depthState = depthState;
}

const unsigned ShaderProgram::getDepthState() const { return this->depthState; }

} // namespace zge