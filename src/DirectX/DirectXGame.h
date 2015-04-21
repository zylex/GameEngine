#ifndef DIRECT_X_GAME_H
#define DIRECT_X_GAME_H

#include <string>
#include <vector>

#include <windows.h>
#include <d3d11.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include "Game.h"

#include "Preprocessors.h"

namespace zge
{

namespace dx
{

class DirectXGame : public Game
{
private:
  HWND hwnd;
  HINSTANCE hinstance;

  IDXGISwapChain* swapChain;

public:
  static DirectXGame* getInstance();
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

  const bool initialise();

  void frame();

private:
  // constructor
  DirectXGame();

  void shutdown();

  const bool initialiseWindow();
  void shutdownWindow();

  const bool initialiseDirectX();

  static bool showDepth;

  void swapBuffers();
  void pollEvents();
};

} // namespace dx

} // namespace zge

#endif
