#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

namespace zge
{
namespace gl
{

unsigned int compileShaders(const char*, const char*);
unsigned int compileShader(const char*);
}
} // namespace zge

#endif