#ifndef DIRECTXRENDERER_H
#define DIRECTXRENDERER_H

#include "Renderer.h"

namespace zge
{

namespace dx
{

class DirectXRenderer : public Renderer
{
private:
public:
  static DirectXRenderer* getInstance();
  // destructor
  virtual ~DirectXRenderer() NOEXCEPT;
  // copy constructor
  DirectXRenderer(const DirectXRenderer&);
  // assignment operator
  DirectXRenderer& operator=(const DirectXRenderer&);
  // move constructor (C++11)
  DirectXRenderer(DirectXRenderer&&) NOEXCEPT;
  // move assignment operator (C++11)
  DirectXRenderer& operator=(DirectXRenderer&&) NOEXCEPT;

private:
  // constructor
  DirectXRenderer();
};

} // namespace dx

} // namespace zge

#endif