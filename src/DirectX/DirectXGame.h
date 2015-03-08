#ifdef DIRECT_X
#ifndef DIRECT_X_GAME_H
#define DIRECT_X_GAME_H

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <string>

#include <windows.h>
#include <d3d11.h>

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
  LPCWSTR m_applicationName;

  // bool m_vsyncEnabled;
  IDXGISwapChain* m_swapChain;
  ID3D11Device* m_device;
  ID3D11DeviceContext* m_deviceContext;

  static bool showDepth;

  const bool initialiseWindow();
  const bool initialiseDirectX();
  void shutdownWindow();
};
} // namespace dx
} // namespace zge

#endif
#endif