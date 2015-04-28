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
  ID3D11DeviceContext* deviceContext;

  unsigned currentInstanceDataSize;

  // unsigned currentProgramId;

  // unsigned currentComputeShaderId;
  // unsigned currentVertexShaderId;
  // unsigned currentGeometryShaderId;
  // unsigned currentDomainShaderId;
  // unsigned currentHullShaderId;
  // unsigned currentPixelShaderId;
  // unsigned currentInputLayoutId;

  unsigned currentDepthStateId;
  unsigned currentRasterStateId;

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

  // TODO: implement state changing methods (depth/culling etc)
  void enableDepth();
  //  void enableReadDepth();
  //  void enableWriteDepth();
  void disableDepth();
  //  void enableAlphaBlending();
  //  void disableAlphaBlending();
  void enableAntiClockwiseCulling();
  void disableCulling();

  void setDeviceContext(ID3D11DeviceContext*);

  void setSamplers(ID3D11SamplerState* []) const;

private:
  // constructor
  DirectXRenderer();

  void bindConstantBuffers(const unsigned, const unsigned,
                           std::vector<ID3D11Buffer*>&);

  void setDepthState(const unsigned);
  void setRasterState(const unsigned);
};

} // namespace dx

} // namespace zge

#endif
