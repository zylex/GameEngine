#ifndef DIRECTXRESOURCES_H
#define DIRECTXRESOURCES_H

#include <d3d11.h>
#include <vector>

namespace zge
{

namespace dx
{

struct Program
{
  unsigned computeShader;
  unsigned vertexShader;
  unsigned geometryShader;
  unsigned hullShader;
  unsigned domainShader;
  unsigned pixelShader;
  unsigned inputLayout;
};

struct Mesh
{
  unsigned numberOfIndices;
  unsigned vertexBufferId;
  unsigned indexBufferId;
  unsigned normalBufferId;
  unsigned textureCoordinatesBufferId;
  unsigned tangentBufferId;
  unsigned bitangentBufferId;
};

struct Output
{
  ID3D11DepthStencilView* depthTarget;
  std::vector<ID3D11RenderTargetView*> outputTargets;
};

enum BlendStates
{
  ALPHA,
  NO_ALPHA
};

enum DepthState
{
  ENABLED,
  READ,
  DISABLED
};

enum RasterStates
{
  COUNTER_CLOCKWISE,
  NO_CULLING
};

} // namespace dx

} // namespace zge

#endif
