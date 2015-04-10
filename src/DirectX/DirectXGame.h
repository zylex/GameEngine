#ifndef DIRECT_X_GAME_H
#define DIRECT_X_GAME_H

#include <string>
#include <vector>

#include <windows.h>
#include <d3d11.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <glm/glm.hpp>

#include "Preprocessors.h"

namespace zge
{
namespace dx
{
class DirectXGame
{
private:
  std::string _applicationName;
  HWND _hwnd;
  HINSTANCE _hinstance;

  // bool _vsyncEnabled;
  IDXGISwapChain* _swapChain;
  ID3D11Device* _device;
  ID3D11DeviceContext* _deviceContext;
  ID3D11DepthStencilState* _depthDefault;
  // ID3D11DepthStencilState* _depthOff;

  ID3D11ShaderResourceView* _depthMapTexture;
  ID3D11DepthStencilView* _depthTarget;
  ID3D11RenderTargetView* _backBuffer;
  ID3D11SamplerState* _sampler;

  ID3D11Buffer* _vertexBuffer;
  ID3D11Buffer* _indexBuffer;
  ID3D11Buffer* _instanceBuffer;
  ID3D11Buffer* _matrixBuffer;
  ID3D11Buffer* _showDepthBuffer;

public:
  // constructor
  DirectXGame(std::string);
  // destructor
  virtual ~DirectXGame() NOEXCEPT;
  // copy constructor
  DirectXGame(const DirectXGame& other);
  // assignment operator
  DirectXGame& operator=(const DirectXGame& other);
  // move constructor (C++11)
  DirectXGame(DirectXGame&& other) NOEXCEPT;
  // move assignment operator (C++11)
  DirectXGame& operator=(DirectXGame&& other) NOEXCEPT;

  const int run();

  static LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
  static bool showDepth;

  const bool initialiseWindow();
  const bool initialiseDirectX();
  ID3D11Buffer* createVertexBuffer(const std::vector<glm::vec3>) const;
  ID3D11Buffer* createIndexBuffer(const std::vector<unsigned long>) const;
  ID3D11Buffer* createInstanceBuffer(const unsigned int) const;
  void shutdownWindow();
};

} // namespace dx

} // namespace zge

#endif
