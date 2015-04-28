#ifdef DIRECT_X
#include "DirectXMaterial.h"

namespace zge
{

namespace dx
{

DirectXMaterial::DirectXMaterial()
{
  // constructor
}

DirectXMaterial::~DirectXMaterial() NOEXCEPT
{
  // destructor
}

DirectXMaterial::DirectXMaterial(const DirectXMaterial& other)
{
  // copy constructor
}

DirectXMaterial& DirectXMaterial::operator=(const DirectXMaterial& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

DirectXMaterial::DirectXMaterial(DirectXMaterial&& other) NOEXCEPT
{
  // move constructor (C++11)
}

DirectXMaterial& DirectXMaterial::operator=(DirectXMaterial&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new DirectXMaterial(other));
}

} // namespace dx

} // namespace zge

#endif
