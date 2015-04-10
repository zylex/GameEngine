#ifndef DIRECT_X_SHADER_H
#define DIRECT_X_SHADER_H

#include <stddef.h>
#include <windows.h>
#include <string>

namespace zge
{
namespace dx
{
const void* CompileShader(const std::string, size_t&);
}
}

#endif