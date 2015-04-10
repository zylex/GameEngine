#ifdef DIRECT_X
#include "DirectXRenderer.h"

namespace zge
{

IRenderer* IRenderer::getInstance()
{
  return dx::DirectXRenderer::getInstance();
}

namespace dx
{

DirectXRenderer* DirectXRenderer::getInstance()
{
  static DirectXRenderer instance;
  return instance;
}

DirectXRenderer::DirectXRenderer()
{
  // constructor
}

DirectXRenderer::~DirectXRenderer() NOEXCEPT
{
  // destructor
}

DirectXRenderer::DirectXRenderer(const DirectXRenderer& other)
{
  // copy constructor
}

DirectXRenderer& DirectXRenderer::operator=(const DirectXRenderer& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

DirectXRenderer::DirectXRenderer(DirectXRenderer&& other) NOEXCEPT
{
  // move constructor (C++11)
}

DirectXRenderer& DirectXRenderer::operator=(DirectXRenderer&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new DirectXRenderer(other));
}

} // namespace dx

} // namespace zge

#endif