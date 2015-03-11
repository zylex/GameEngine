#ifdef DIRECT_X
#ifndef DIRECT_X_GAME_H
#define DIRECT_X_GAME_H

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <string>
#include <vector>

#include <windows.h>
#include <d3d11.h>

#include <glm/glm.hpp>

#include "Preprocessors.h"

namespace zge
{
namespace dx
{
class DirectXGame
{
public:
  // constructor
  DirectXGame(std::string);
  // destructor
  virtual ~DirectXGame() noexcept;
  // copy constructor
  DirectXGame(const DirectXGame& other);
  // assignment operator
  DirectXGame& operator=(const DirectXGame& other);
  // move constructor (C++11)
  DirectXGame(DirectXGame&& other) noexcept;
  // move assignment operator (C++11)
  DirectXGame& operator=(DirectXGame&& other) noexcept;

  const int run();

  static LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
  std::string m_applicationName;
  HWND m_hwnd;
  HINSTANCE m_hinstance;

  // bool m_vsyncEnabled;
  IDXGISwapChain* m_swapChain;
  ID3D11Device* m_device;
  ID3D11DeviceContext* m_deviceContext;
  ID3D11DepthStencilState* m_depthDefault;
  //ID3D11DepthStencilState* m_depthOff;

  ID3D11ShaderResourceView* m_depthMapTexture;
  ID3D11DepthStencilView* m_depthTarget;
  ID3D11RenderTargetView* m_backBuffer;
  ID3D11SamplerState* m_sampler;

  ID3D11Buffer* m_vertexBuffer;
  ID3D11Buffer* m_indexBuffer;
  ID3D11Buffer* m_instanceBuffer;

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
#endif
