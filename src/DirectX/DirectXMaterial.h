#ifndef DIRECTXMATERIAL_H
#define DIRECTXMATERIAL_H

#include "Material.h"

namespace zge
{

namespace dx
{

class DirectXMaterial : public Material
{
private:
public:
  // constructor
  DirectXMaterial();
  // destructor
  virtual ~DirectXMaterial() NOEXCEPT;
  // copy constructor
  DirectXMaterial(const DirectXMaterial&);
  // assignment operator
  DirectXMaterial& operator=(const DirectXMaterial&);
  // move constructor (C++11)
  DirectXMaterial(DirectXMaterial&&) NOEXCEPT;
  // move assignment operator (C++11)
  DirectXMaterial& operator=(DirectXMaterial&&) NOEXCEPT;

private:
};

} // namespace dx

} // namespace zge

#endif