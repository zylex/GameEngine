#ifdef DIRECT_X
#include "DirectXGame.h"
#include "IGame.h"

namespace zge
{
namespace dx
{
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

  DirectXResourceManager::getInstance()->setDevice(m_device);

  ID3D11Texture2D* texture2d[5];

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

  D3D11_TEXTURE2D_DESC textureDescription = CD3D11_TEXTURE2D_DESC(
      DXGI_FORMAT_R32G32B32A32_FLOAT, SCREEN_WIDTH(), SCREEN_HEIGHT(), 1, 1,
      D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

  if (FAILED(
          m_device->CreateTexture2D(&textureDescription, NULL, &texture2d[1])))
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_texture1[] = "Color Texture";
  texture2d[1]->SetPrivateData(WKPDID_D3DDebugObjectName,
                               sizeof(str_texture1) - 1, str_texture1);
#endif

  if (FAILED(
          m_device->CreateTexture2D(&textureDescription, NULL, &texture2d[2])))
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_texture2[] = "Normal Texture";
  texture2d[2]->SetPrivateData(WKPDID_D3DDebugObjectName,
                               sizeof(str_texture2) - 1, str_texture2);
#endif

  textureDescription.Format = DXGI_FORMAT_R32_TYPELESS;
  textureDescription.BindFlags =
      D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc =
      CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D,
                                     DXGI_FORMAT_R32_FLOAT);

  if (FAILED(
          m_device->CreateTexture2D(&textureDescription, NULL, &texture2d[3])))
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_texture3[] = "Light Depth Texture";
  texture2d[3]->SetPrivateData(WKPDID_D3DDebugObjectName,
                               sizeof(str_texture3) - 1, str_texture3);
#endif

  if (FAILED(
          m_device->CreateTexture2D(&textureDescription, NULL, &texture2d[4])))
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_texture4[] = "Depth Texture";
  texture2d[4]->SetPrivateData(WKPDID_D3DDebugObjectName,
                               sizeof(str_texture4) - 1, str_texture4);
#endif

  _depthShader = new DirectXDepthShader;
  if (NOT _depthShader)
  {
    return false;
  }

  if (NOT _depthShader->initialise(m_device, m_deviceContext, &texture2d[4]))
  {
    return false;
  }

  _deferredShader = new DirectXDeferredShader;
  if (NOT _deferredShader)
  {
    return false;
  }

  if (NOT _deferredShader->initialise(m_device, m_deviceContext, &texture2d[1]))
  {
    return false;
  }

  _deferredLightShader = new DirectXDeferredLightShader;
  if (NOT _deferredLightShader)
  {
    return false;
  }

  if (NOT _deferredLightShader->initialise(m_device, m_deviceContext,
                                           texture2d))
  {
    return false;
  }

  // for (int i = 0; i < 5; i++)
  //{
  //  if (texture2d[i])
  //  {
  //    texture2d[i]->Release();
  //    texture2d[i] = NULL;
  //  }
  //}

  DirectX::CommonStates* statesFactory = new DirectX::CommonStates(m_device);

  _depthStencilStates[DepthState::DEFAULT] = statesFactory->DepthDefault();
  if (NOT _depthStencilStates[DepthState::DEFAULT])
  {
    return false;
  }

  _depthStencilStates[DepthState::READ] = statesFactory->DepthRead();
  if (NOT _depthStencilStates[DepthState::READ])
  {
    return false;
  }

  _depthStencilStates[DepthState::OFF] = statesFactory->DepthNone();
  if (NOT _depthStencilStates[DepthState::OFF])
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_Default[] = "DepthState Default";
  _depthStencilStates[DepthState::DEFAULT]->SetPrivateData(
      WKPDID_D3DDebugObjectName, sizeof(str_Default) - 1, str_Default);

  const char strRead[] = "DepthState Read";
  _depthStencilStates[DepthState::READ]->SetPrivateData(
      WKPDID_D3DDebugObjectName, sizeof(strRead) - 1, strRead);

  const char strOffr[] = "DepthState Off";
  _depthStencilStates[DepthState::OFF]->SetPrivateData(
      WKPDID_D3DDebugObjectName, sizeof(strOffr) - 1, strOffr);
#endif

  _rasterStates[RasterStates::COUNTER_CLOCKWISE] =
      statesFactory->CullCounterClockwise();
  if (NOT _rasterStates[RasterStates::COUNTER_CLOCKWISE])
  {
    return false;
  }

  _rasterStates[RasterStates::NO_CULLING] = statesFactory->CullNone();
  if (NOT _rasterStates[RasterStates::NO_CULLING])
  {
    return false;
  }

#if defined(_DEBUG)
  const char strCounterClockwise[] = "RasterState COUNTER_CLOCKWISE";
  _rasterStates[RasterStates::COUNTER_CLOCKWISE]->SetPrivateData(
      WKPDID_D3DDebugObjectName, sizeof(strCounterClockwise) - 1,
      strCounterClockwise);

  const char strNoCull[] = "RasterState NO_CULLING";
  _rasterStates[RasterStates::NO_CULLING]->SetPrivateData(
      WKPDID_D3DDebugObjectName, sizeof(strNoCull) - 1, strNoCull);
#endif

  _blendStates[BlendStates::ALPHA] = statesFactory->AlphaBlend();
  if (NOT _blendStates[BlendStates::ALPHA])
  {
    return false;
  }

  _blendStates[BlendStates::NO_ALPHA] = statesFactory->Opaque();
  if (NOT _blendStates[BlendStates::NO_ALPHA])
  {
    return false;
  }

#if defined(_DEBUG)
  const char strALPHA[] = "BlendStates::ALPHA";
  _blendStates[BlendStates::ALPHA]->SetPrivateData(
      WKPDID_D3DDebugObjectName, sizeof(strALPHA) - 1, strALPHA);

  const char strNO_ALPHA[] = "BlendStates::NO_ALPHA";
  _blendStates[BlendStates::NO_ALPHA]->SetPrivateData(
      WKPDID_D3DDebugObjectName, sizeof(strNO_ALPHA) - 1, strNO_ALPHA);
#endif

  D3D11_VIEWPORT viewport = CD3D11_VIEWPORT(0.0f, 0.0f, (float)SCREEN_WIDTH(),
                                            (float)SCREEN_HEIGHT());

  m_deviceContext->RSSetViewports(1, &viewport);

  _samplers[0] = statesFactory->LinearWrap();
  if (NOT _samplers[0])
  {
    return false;
  }

  _samplers[1] = statesFactory->PointClamp();
  if (NOT _samplers[1])
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_samplers0[] = "LinearWrap";
  _samplers[0]->SetPrivateData(WKPDID_D3DDebugObjectName,
                               sizeof(str_samplers0) - 1, str_samplers0);

  const char str_samplers1[] = "PointClamp";
  _samplers[1]->SetPrivateData(WKPDID_D3DDebugObjectName,
                               sizeof(str_samplers1) - 1, str_samplers1);
#endif

  m_deviceContext->PSSetSamplers(0, 2, _samplers);

  float fieldOfView = (float)XM_PI / 4.0f;
  float screenAspect = (float)SCREEN_WIDTH() / (float)SCREEN_WIDTH();

  XMStoreFloat4x4(&_projectionMatrix,
                  XMMatrixPerspectiveFovLH(fieldOfView, screenAspect,
                                           screenNear, screenDepth));
  XMStoreFloat4x4(&_worldMatrix, XMMatrixIdentity());

  setModelFactory(new DirectXModelFactory(this));

  _blendFactor[0] = 0.0f;
  _blendFactor[1] = 0.0f;
  _blendFactor[2] = 0.0f;
  _blendFactor[3] = 0.0f;

  m_deviceContext->IASetPrimitiveTopology(
      D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  _buffers[Buffers::MATRIX_BUFFER] =
      DirectXResourceManager::getInstance()->getMatrixBuffer();
  _buffers[Buffers::INVERSE_PROJECTION_BUFFER] =
      DirectXResourceManager::getInstance()->getInverseProjectionMatrixBuffer();
  _buffers[Buffers::LIGHT_BUFFER] =
      DirectXResourceManager::getInstance()->getLightBuffer();
  _buffers[Buffers::INSTANCE_BUFFER] =
      DirectXResourceManager::getInstance()->getInstanceBuffer();

  turnOnCulling();
  turnOnDepthBuffer();
  turnOffAlphaBlending();

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