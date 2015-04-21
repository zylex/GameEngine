#ifndef DEPTHSHADER_H
#define DEPTHSHADER_H

#include "Preprocessors.h"

#include "MeshShader.h"

namespace zge
{

class DepthShader : public MeshShader
{
private:
  unsigned depthOutput;
  unsigned depthTexture;

public:
  // constructor
  DepthShader();
  // destructor
  virtual ~DepthShader() NOEXCEPT;
  // copy constructor
  DepthShader(const DepthShader&);
  // assignment operator
  DepthShader& operator=(const DepthShader&);
  // move constructor (C++11)
  DepthShader(DepthShader&&) NOEXCEPT;
  // move assignment operator (C++11)
  DepthShader& operator=(DepthShader&&) NOEXCEPT;

  const bool initialise();
  void prepare();

  const unsigned getDepthTexture();
};

} // namespace zge

#endif
