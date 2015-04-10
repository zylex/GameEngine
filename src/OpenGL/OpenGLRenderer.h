#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "Renderer.h"

namespace zge
{

namespace gl
{

class OpenGLRenderer : public Renderer
{
private:
  unsigned currentShaderProgramId;

public:
  static OpenGLRenderer* getInstance();
  // destructor
  virtual ~OpenGLRenderer() noexcept;
  // copy constructor
  OpenGLRenderer(const OpenGLRenderer&);
  // assignment operator
  OpenGLRenderer& operator=(const OpenGLRenderer&);
  // move constructor (C++11)
  OpenGLRenderer(OpenGLRenderer&&) noexcept;
  // move assignment operator (C++11)
  OpenGLRenderer& operator=(OpenGLRenderer&&) noexcept;

  const bool initialise();

  void setShaderProgram(const unsigned);

  void setConstants(const void**, const std::vector<unsigned long>,
                    const unsigned);
  void setConstant(const unsigned, const void*, const unsigned long,
                   const unsigned);

  void executeInstancedShader(const unsigned, const void*, const unsigned long,
                              const unsigned);
  void executeComputeShader();
  void executeShader();

  void setTextures(const std::vector<unsigned>, const std::vector<unsigned>,
                   const unsigned);
  void setTexture(const unsigned, const unsigned, const unsigned,
                  const unsigned);

  void setOutput(const unsigned);

  void enableDepth();
  void disableDepth();

private:
  // constructor
  OpenGLRenderer();
};

} // namespace gl

} // namespace zge

#endif