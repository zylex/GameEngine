#ifndef SHADERTYPE_H
#define SHADERTYPE_H

namespace zge
{

enum ShaderType
{
  SHADERTYPE_NULL,
  COMPUTE_SHADER,
  VERTEX_SHADER,
  GEOMETRY_SHADER,
  HULL_SHADER,   // Tesselation Control Shader
  DOMAIN_SHADER, // Tesselation Evaluation Shader
  PIXEL_SHADER   // Fragment Shader
};

} // namespace zge

#endif
