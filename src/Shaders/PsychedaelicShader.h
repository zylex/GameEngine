#ifndef PSYCHEDAELICSHADER_H
#define PSYCHEDAELICSHADER_H

#include "Preprocessors.h"

#include "MeshShader.h"

namespace zge
{

class PsychedaelicShader : public MeshShader
{
private:
  bool showDepth;
  unsigned depthTextureId;

public:
  // constructor
  PsychedaelicShader(unsigned);
  // destructor
  virtual ~PsychedaelicShader() NOEXCEPT;
  // copy constructor
  PsychedaelicShader(const PsychedaelicShader&);
  // assignment operator
  PsychedaelicShader& operator=(const PsychedaelicShader&);
  // move constructor (C++11)
  PsychedaelicShader(PsychedaelicShader&&) NOEXCEPT;
  // move assignment operator (C++11)
  PsychedaelicShader& operator=(PsychedaelicShader&&) NOEXCEPT;

  void addInstance(IGameObject*);

  const bool initialise();
  void prepare();
  void finish();

  void switchShowDepth();
};

} // namespace zge

#endif
