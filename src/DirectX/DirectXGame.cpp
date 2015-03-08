#ifdef DIRECT_X

#include <vector>
#include <glm/glm.hpp>

#include "DirectXGame.h"
#include "IGame.h"

namespace zge
{
namespace dx
{

using namespace std;
using namespace glm;

DirectXGame::DirectXGame(string applicationName)
{
  // constructor
  m_applicationName = applicationName;
}

DirectXGame::~DirectXGame() noexcept
{
  shutdownWindow();
  // destructor
}

DirectXGame::DirectXGame(const DirectXGame& other)
{
  // copy constructor
}

DirectXGame& DirectXGame::operator=(const DirectXGame& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

DirectXGame::DirectXGame(DirectXGame&& other) noexcept
{
  // move constructor (C++11)
}

DirectXGame& DirectXGame::operator=(DirectXGame&& other) noexcept
{
  // move assignment operator (C++11)
  return *(new DirectXGame(other));
}

const bool DirectXGame::initialiseWindow()
{
  // TODO: Error handling and return false.
  // adapted from rastertek.com
  int width, height;
  WNDCLASSEX windowClass;
  DEVMODE screenSettings;
  int posX, posY;

  m_hinstance = GetModuleHandle(NULL);

  // Setup the windows class with default settings.
  windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  windowClass.lpfnWndProc = DirectXGame::MessageHandler;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = m_hinstance;
  windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  windowClass.hIconSm = windowClass.hIcon;
  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  windowClass.lpszMenuName = NULL;
  windowClass.lpszClassName = m_applicationName;
  windowClass.cbSize = sizeof(WNDCLASSEX);

  // Register the window class.
  RegisterClassEx(&windowClass);

  // Determine the resolution of the clients desktop screen.
  width = GetSystemMetrics(SM_CXSCREEN);
  height = GetSystemMetrics(SM_CYSCREEN);

  // Setup the screen settings depending on whether it is running in full screen
  // or in windowed mode.
  if (FULL_SCREEN)
  {
    // If full screen set the screen to maximum size of the users desktop and
    // 32bit.
    memset(&screenSettings, 0, sizeof(screenSettings));
    screenSettings.dmSize = sizeof(screenSettings);
    screenSettings.dmPelsWidth = (unsigned long)width;
    screenSettings.dmPelsHeight = (unsigned long)height;
    screenSettings.dmBitsPerPel = 32;
    screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // Change the display settings to full screen.
    ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);

    // Set the position of the window to the top left corner.
    posX = posY = 0;
  }
  else
  {
    // If windowed then set it to 800x600 resolution.
    width = WINDOWED_WIDTH;
    height = WINDOWED_HEIGHT;

    // Place the window in the middle of the screen.
    posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
  }

  SCREEN_WIDTH(width);
  SCREEN_HEIGHT(height);

  // Create the window with the screen settings and get the handle to it.
  m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
                          WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX,
                          posY, width, height, NULL, NULL, m_hinstance, NULL);

  // Bring the window up on the screen and set it as main focus.
  ShowWindow(m_hwnd, SW_SHOW);
  SetForegroundWindow(m_hwnd);
  SetFocus(m_hwnd);

  // Hide the mouse cursor.
  // ShowCursor(false);
  ShowCursor(true);

  return true;
}

const bool DirectXGame::initialiseDirectX()
{
  // adapted from rastertek.com
  IDXGIFactory* factory;
  if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
  {
    return false;
  }

  IDXGIAdapter* adapter;
  if (FAILED(factory->EnumAdapters(0, &adapter)))
  {
    return false;
  }

  IDXGIOutput* adapterOutput;
  if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
  {
    return false;
  }

  unsigned int numModes;
  if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
                                               DXGI_ENUM_MODES_INTERLACED,
                                               &numModes, NULL)))
  {
    return false;
  }

  DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
  if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
                                               DXGI_ENUM_MODES_INTERLACED,
                                               &numModes, displayModeList)))
  {
    return false;
  }

  unsigned int numerator, denominator;
  for (unsigned int i = 0; i < numModes; i++)
  {
    if (displayModeList[i].Width == (unsigned int)SCREEN_WIDTH())
    {
      if (displayModeList[i].Height == (unsigned int)SCREEN_HEIGHT())
      {
        numerator = displayModeList[i].RefreshRate.Numerator;
        denominator = displayModeList[i].RefreshRate.Denominator;
      }
    }
  }

  delete[] displayModeList;
  displayModeList = NULL;

  adapterOutput->Release();
  adapterOutput = NULL;

  adapter->Release();
  adapter = NULL;

  factory->Release();
  factory = NULL;

  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

  swapChainDesc.BufferCount = 1;

  swapChainDesc.BufferDesc.Width = SCREEN_WIDTH();
  swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT();

  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  vsyncEnabled = true;
  if (vsyncEnabled)
  {
    swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
  }
  else
  {
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
  }

  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

  swapChainDesc.OutputWindow = m_hwnd;

  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.SampleDesc.Quality = 0;

  if (FULL_SCREEN)
  {
    swapChainDesc.Windowed = false;
  }
  else
  {
    swapChainDesc.Windowed = true;
  }

  swapChainDesc.BufferDesc.ScanlineOrdering =
      DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  swapChainDesc.Flags = 0;

  D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

  if (FAILED(D3D11CreateDeviceAndSwapChain(
          NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
          D3D11_SDK_VERSION, &swapChainDesc, &mm_swapChain, &m_device, NULL,
          &m_deviceContext)))
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_Swap[] = "Swap Chain";
  m_swapChain->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(str_Swap) - 1,
                              str_Swap);

  const char strm_device[] = "Device";
  m_device->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(strm_device) - 1,
                           strm_device);

  const char strm_deviceContext[] = "DeviceContext";
  m_deviceContext->SetPrivateData(WKPDID_D3DDebugObjectName,
                                  sizeof(strm_deviceContext) - 1,
                                  strm_deviceContext);
#endif

  ID3D11Texture2D* texture2d[2];

  if (FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                    (LPVOID*)&texture2d[0])))
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_texture0[] = "BackBuffer Texture";
  texture2d[0]->SetPrivateData(WKPDID_D3DDebugObjectName,
                               sizeof(str_texture0) - 1, str_texture0);
#endif

  if (FAILED(device->CreateRenderTargetView(textures[0], NULL, &m_backBuffer)))
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_backBuffer[] = "BackBuffer";
  _backBuffer->SetPrivateData(WKPDID_D3DDebugObjectName,
                              sizeof(str_backBuffer) - 1, str_backBuffer);
#endif

  D3D11_TEXTURE2D_DESC textureDescription;
  ZeroMemory(textureDescription, sizeof(textureDescription));
  textureDescription.Width = SCREEN_WIDTH();
  textureDescription.Height = SCREEN_HEIGHT();
  textureDescription.MipLevels = 1;
  textureDescription.ArraySize = 1;
  textureDescription.Format = DXGI_FORMAT_R32_TYPELESS;
  textureDescription.SampleDesc.Count = 1;
  textureDescription.SampleDesc.Quality = 0;
  textureDescription.Usage = D3D11_USAGE_DEFAULT;
  textureDescription.BindFlags =
      D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  textureDescription.CPUAccessFlags = 0;
  textureDescription.MiscFlags = 0;

  if (FAILED(
          m_device->CreateTexture2D(&textureDescription, NULL, &texture2d[1])))
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_texture4[] = "Depth Texture";
  texture2d[1]->SetPrivateData(WKPDID_D3DDebugObjectName,
                               sizeof(str_texture4) - 1, str_texture4);
#endif

  D3D11_DEPTH_STENCIL_DESC desc;
  ZeroMemory(&desc, sizeof(desc));

  desc.DepthEnable = true;
  desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

  desc.StencilEnable = false;
  desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
  desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

  desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

  desc.BackFace = desc.FrontFace;

  if (FAILED(m_device->CreateDepthStencilState(&desc, m_depthDefault)))
  {
    return false;
  }

  desc.DepthEnable = false;
  desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
  if (FAILED(m_device->CreateDepthStencilState(&desc, m_depthOff)))
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_Default[] = "DepthState Default";
  m_depthDefault->SetPrivateData(WKPDID_D3DDebugObjectName,
                                 sizeof(str_Default) - 1, str_Default);

  const char strOffr[] = "DepthState Off";
  m_depthOff->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(strOffr) - 1,
                             strOffr);
#endif

  D3D11_VIEWPORT viewport = CD3D11_VIEWPORT(0.0f, 0.0f, (float)SCREEN_WIDTH(),
                                            (float)SCREEN_HEIGHT());

  m_deviceContext->RSSetViewports(1, &viewport);

  m_sampler = statesFactory->PointClamp();
  if (NOT m_sampler)
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_samplers1[] = "PointClamp";
  m_sampler->SetPrivateData(WKPDID_D3DDebugObjectName,
                            sizeof(str_samplers1) - 1, str_samplers1);
#endif

  m_deviceContext->PSSetSamplers(0, 1, m_sampler);

  m_deviceContext->IASetPrimitiveTopology(
      D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
  depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
  depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  depthStencilViewDesc.Flags = 0;
  depthStencilViewDesc.Texture2D.MipSlice = 0;

  if (FAILED(m_device->CreateDepthStencilView(
          texture2d[1], &depthStencilViewDesc, &m_depthTarget)))
  {
    return false;
  }

  D3D11_SHADER_RESOURCE_VIEW_DESC depthTargetDesc;
  depthTargetDesc.Format = DXGI_FORMAT_R32_FLOAT;
  depthTargetDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  depthTargetDesc.Texture2D.MostDetailedMip = 0;
  depthTargetDesc.Texture2D.MipLevels = 1;

  if (FAILED(device->CreateShaderResourceView(texture2d[1], &depthTargetDesc,
                                              &m_depthMapTexture)))
  {
    return false;
  }

  vector<vec3> vertices;
  vertices.push_back(vec3(-1.0f, -1.0f, 0.0f));
  vertices.push_back(vec3(-1.0f, 1.0f, 0.0f));
  vertices.push_back(vec3(1.0f, 1.0f, 0.0f));
  vertices.push_back(vec3(1.0f, -1.0f, 0.0f));

  _buffers[Buffers::MATRIX_BUFFER] =
      DirectXResourceManager::getInstance()->getMatrixBuffer();
  _buffers[Buffers::INVERSE_PROJECTION_BUFFER] =
      DirectXResourceManager::getInstance()->getInverseProjectionMatrixBuffer();
  _buffers[Buffers::LIGHT_BUFFER] =
      DirectXResourceManager::getInstance()->getLightBuffer();
  _buffers[Buffers::INSTANCE_BUFFER] =
      DirectXResourceManager::getInstance()->getInstanceBuffer();

  for (int i = 0; i < 2; i++)
  {
    if (texture2d[i])
    {
      texture2d[i]->Release();
      texture2d[i] = NULL;
    }
  }

  return true;
}

void DirectXGame::shutdownWindow()
{
  // Show the mouse cursor.
  ShowCursor(true);

  // Fix the display settings if leaving full screen mode.
  if (FULL_SCREEN)
  {
    ChangeDisplaySettings(NULL, 0);
  }

  // Remove the window.
  DestroyWindow(m_hwnd);
  m_hwnd = NULL;

  // Remove the application instance.
  UnregisterClass(m_applicationName, m_hinstance);
  m_hinstance = NULL;
}

} // namspace dx
} // namespace zge
#endif