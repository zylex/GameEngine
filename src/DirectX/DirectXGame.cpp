#ifdef DIRECT_X

#include <iostream>

#include <glm/gtx/transform.hpp>

#include "DirectXGame.h"

#include "DirectXInputManager.h"
#include "DirectXRenderer.h"
#include "DirectXResourceManager.h"

namespace zge
{

IGame* IGame::getInstance() { return dx::DirectXGame::getInstance(); }

namespace dx
{

DirectXGame* DirectXGame::getInstance()
{
  static DirectXGame instance;
  return &instance;
}

bool DirectXGame::showDepth = false;

DirectXGame::DirectXGame()
{
  // constructor
}

DirectXGame::~DirectXGame() NOEXCEPT
{
  // destructor
  this->shutdown();
}

DirectXGame::DirectXGame(const DirectXGame& other)
{
  // copy constructor
}

DirectXGame& DirectXGame::operator=(const DirectXGame& rhs)
{
  // assignement operator
  if (this IS & rhs)
  {
    return *this;
  }

  return *this;
}

DirectXGame::DirectXGame(DirectXGame&& other) NOEXCEPT
{
  // move constructor (C++11)
}

DirectXGame& DirectXGame::operator=(DirectXGame&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new DirectXGame(other));
}

const bool DirectXGame::initialise()
{
  if (not this->initialiseWindow())
  {
    std::cerr << "Failed to initialise window." << std::endl;
    return false;
  }

  if (not this->initialiseDirectX())
  {
    std::cerr << "Failed to initialise Direct X." << std::endl;
    return false;
  }

  return Game::initialise();
}

void DirectXGame::frame()
{
  Game::frame();

  this->swapChain->Present(VSYNC_ENABLED, 0);

  this->pollEvents();
}

const bool DirectXGame::initialiseWindow()
{
  // adapted from rastertek.com

  //this->hinstance = GetModuleHandle(NULL);
  this->hinstance = GetModuleHandle(nullptr);

  // docs say return value is NULL for failure, hopefully this will equal
  // nullptr
  // if (this->hinstance IS NULL)
  if (this->hinstance IS nullptr)
  {
    return false;
  }
  std::string windowTitle = this->getWindowTitle();
  std::wstring applicationName(windowTitle.begin(), windowTitle.end());
  //LPCSTR applicationName = this->getWindowTitle().c_str();

  WNDCLASSEX windowClass;
  // Setup the windows class with default settings.
  windowClass.cbSize = sizeof(WNDCLASSEX);
  windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  windowClass.lpfnWndProc = DirectXInputManager::MessageHandler;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = this->hinstance;
  windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  windowClass.lpszMenuName = NULL;
  //windowClass.lpszClassName = this->getWindowTitle();
  windowClass.lpszClassName = applicationName.c_str();
  //windowClass.lpszClassName = "snowglobe";
  windowClass.hIconSm = windowClass.hIcon;


  // Register the window class.
  //RegisterClassEx(&windowClass);
  if (not RegisterClassEx(&windowClass))
  {
    return false;
  }

  int width, height, posX, posY;

  // Setup the screen settings depending on whether it is running in full screen
  // or in windowed mode.
  if (FULL_SCREEN)
  {
    // Determine the resolution of the client's desktop screen.
    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);
    // If full screen set the screen to maximum size of the users desktop and
    // 32bit.
    DEVMODE screenSettings;
    memset(&screenSettings, 0, sizeof(screenSettings));
    screenSettings.dmSize = sizeof(screenSettings);
    screenSettings.dmPelsWidth = (unsigned long)width;
    screenSettings.dmPelsHeight = (unsigned long)height;
    screenSettings.dmBitsPerPel = 32;
    screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // Change the display settings to full screen.
    if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN)
            IS_NOT DISP_CHANGE_SUCCESSFUL)
    {
      return false;
    }

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
  // this->hwnd =
  //    CreateWindowEx(WS_EX_APPWINDOW, this->getWindowTitle().c_str(),
  //                   this->getWindowTitle().c_str(),
  //                   WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY,
  //                   width, height, NULL, NULL, this->hinstance, NULL);

  this->hwnd = CreateWindow(
    applicationName.c_str(), applicationName.c_str(),
      WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION, posX, posY,
      width, height, nullptr, nullptr, this->hinstance, nullptr);
  // docs say return value is NULL for failure, hopefully this will equal
  // nullptr
  //if (this->hwnd IS NULL)
    if (this->hwnd IS nullptr)
    {
      return false;
    }

  // Bring the window up on the screen and set it as main focus.
  ShowWindow(this->hwnd, SW_SHOW);
  SetForegroundWindow(this->hwnd);
  SetFocus(this->hwnd);

  // Hide the mouse cursor.
  // ShowCursor(false);

  // show the cursor in case it was invisible :P
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
                                               &numModes, nullptr)))
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
    if (displayModeList[i].Width IS(unsigned int)SCREEN_WIDTH())
    {
      if (displayModeList[i].Height IS(unsigned int)SCREEN_HEIGHT())
      {
        numerator = displayModeList[i].RefreshRate.Numerator;
        denominator = displayModeList[i].RefreshRate.Denominator;
      }
    }
  }

  delete[] displayModeList;
  displayModeList = nullptr;

  adapterOutput->Release();
  adapterOutput = nullptr;

  adapter->Release();
  adapter = nullptr;

  factory->Release();
  factory = nullptr;

  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

  swapChainDesc.BufferCount = 1;

  swapChainDesc.BufferDesc.Width = SCREEN_WIDTH();
  swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT();

  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  if (VSYNC_ENABLED)
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

  swapChainDesc.OutputWindow = this->hwnd;

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

  ID3D11Device* device;
  ID3D11DeviceContext* deviceContext;

  if (FAILED(D3D11CreateDeviceAndSwapChain(
          NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
          D3D11_SDK_VERSION, &swapChainDesc, &this->swapChain, &device, NULL,
          &deviceContext)))
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_Swap[] = "Swap Chain";
  this->swapChain->SetPrivateData(WKPDID_D3DDebugObjectName,
                                  sizeof(str_Swap) - 1, str_Swap);

  const char str_device[] = "Device";
  device->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(str_device) - 1,
                         str_device);

  const char str_deviceContext[] = "DeviceContext";
  deviceContext->SetPrivateData(WKPDID_D3DDebugObjectName,
                                sizeof(str_deviceContext) - 1,
                                str_deviceContext);
#endif

  ID3D11Texture2D* texture2d;

  if (FAILED(this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                        (LPVOID*)&texture2d)))
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_texture0[] = "BackBuffer Texture";
  texture2d->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(str_texture0) - 1,
                            str_texture0);
#endif

  DirectXRenderer::getInstance()->setDeviceContext(deviceContext);
  DirectXResourceManager::getInstance()->setDevice(device);

  if (not DirectXResourceManager::getInstance()->setBackBufferTexture(
          texture2d))
  {
    return false;
  }

  //if (not DirectXRenderer::getInstance()->initialise())
  //{
  //  return false;
  //}

  //if (not DirectXResourceManager::getInstance()->initialise())
  //{
  //  return false;
  //}


  SAFE_RELEASE(texture2d);

  // DirectXRenderer::getInstance()->enableDepth();

  return true;
}

// const int DirectXGame::run()
//{
//  initialiseWindow();
//  initialiseDirectX();
//
//  // create shaders
//  ID3D11VertexShader* interpolatedVertexShader;
//  if (FAILED(_device->CreateVertexShader(InterpolatedVS, InterpolatedVS_size,
//                                         NULL, &interpolatedVertexShader)))
//  {
//    return EXIT_FAILURE;
//  }
//
//  ID3D11PixelShader* discardPixelShader;
//  if (FAILED(_device->CreatePixelShader(DiscardPS, DiscardPS_size, NULL,
//                                        &discardPixelShader)))
//  {
//    return EXIT_FAILURE;
//  }
//
//  ID3D11PixelShader* interpolatedPixelShader;
//  if (FAILED(_device->CreatePixelShader(InterpolatedDepthPS,
//                                        InterpolatedDepthPS_size, NULL,
//                                        &interpolatedPixelShader)))
//  {
//    return EXIT_FAILURE;
//  }
//
//  // create input layout
//  D3D11_INPUT_ELEMENT_DESC layoutDescription[] = {
//    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
//      D3D11_INPUT_PER_VERTEX_DATA, 0 },
//    { "WORLD_MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,
//      D3D11_INPUT_PER_INSTANCE_DATA, 1 },
//    { "WORLD_MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
//      D3D11_INPUT_PER_INSTANCE_DATA, 1 },
//    { "WORLD_MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32,
//      D3D11_INPUT_PER_INSTANCE_DATA, 1 },
//    { "WORLD_MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48,
//      D3D11_INPUT_PER_INSTANCE_DATA, 1 }
//  };
//
//  ID3D11InputLayout* inputLayout;
//  if (FAILED(_device->CreateInputLayout(layoutDescription, 5, InterpolatedVS,
//                                        InterpolatedVS_size, &inputLayout)))
//  {
//    return EXIT_FAILURE;
//  }
//
//#if defined(_DEBUG)
//  const char str_inputLayout[] = "InterpolatedVS inputLayout";
//  inputLayout->SetPrivateData(WKPDID_D3DDebugObjectName,
//                              sizeof(str_inputLayout) - 1, str_inputLayout);
//#endif
//
//  // create buffers
//  D3D11_BUFFER_DESC constantBufferDescription;
//
//  constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
//  constantBufferDescription.ByteWidth = sizeof(glm::mat4);
//  constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//  constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//  constantBufferDescription.MiscFlags = 0;
//  constantBufferDescription.StructureByteStride = 0;
//
//  if (FAILED(_device->CreateBuffer(&constantBufferDescription, NULL,
//                                   &_matrixBuffer)))
//  {
//    return EXIT_FAILURE;
//  }
//
//#if defined(_DEBUG)
//  const char str[] = "MatrixBuffer";
//  _matrixBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(str) - 1,
//                                str);
//#endif
//
//  constantBufferDescription.ByteWidth = 16;
//
//  if (FAILED(_device->CreateBuffer(&constantBufferDescription, NULL,
//                                   &_showDepthBuffer)))
//  {
//    return EXIT_FAILURE;
//  }
//
//#if defined(_DEBUG)
//  const char ShowDepthBufferstr[] = "ShowDepthBuffer";
//  _showDepthBuffer->SetPrivateData(WKPDID_D3DDebugObjectName,
//                                   sizeof(ShowDepthBufferstr) - 1, str);
//#endif
//
//  std::vector<glm::vec3> vertices;
//  vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
//  vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
//  vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
//  vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
//
//  _vertexBuffer = createVertexBuffer(vertices);
//  if (not _vertexBuffer)
//  {
//    return EXIT_FAILURE;
//  }
//  _indexBuffer = createIndexBuffer({ 0, 1, 2, 2, 3, 0 });
//  _instanceBuffer = createInstanceBuffer(4);
//
//  std::vector<glm::vec3> instancePositions({
//      glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f),
//      glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
//  });
//
//  // needed to set the vertex and instance buffers
//  unsigned int stride[2];
//  stride[0] = sizeof(glm::vec3);
//  stride[1] = sizeof(glm::mat4);
//  unsigned int offset[2];
//  offset[0] = 0;
//  offset[1] = 0;
//  ID3D11Buffer* buf[2];
//  buf[0] = _vertexBuffer;
//  buf[1] = _instanceBuffer;
//
//  _deviceContext->VSSetShader(interpolatedVertexShader, nullptr, 0);
//  _deviceContext->PSSetShaderResources(0, 1, &_depthMapTexture);
//
//  // loop
//  bool running = true;
//  while (running)
//  {
//    MSG msg;
//    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//    {
//      TranslateMessage(&msg);
//      DispatchMessage(&msg);
//    }
//
//    if (msg.message == WM_QUIT)
//    {
//      running = false;
//    }
//    else
//    {
//      std::vector<glm::mat4> instanceData(4);
//
//      static float scale = 0.2f;
//      glm::mat4 worldMatrix;
//      worldMatrix = glm::scale(worldMatrix, glm::vec3(scale, scale, scale));
//
//      static float translation = 0.0f;
//      translation += 0.01f;
//      for (int i = 0; i < 4; ++i)
//      {
//        glm::mat4 instDat;
//        instDat =
//            glm::translate(instDat, instancePositions[i]) *
//            glm::translate(instDat, glm::vec3(sinf(translation) * 0.2f * 1.4f,
//                                              cosf(translation) * 0.2f,
//                                              0.0f));
//        instanceData[i] = glm::transpose(instDat);
//      }
//
//      // set render target
//      _deviceContext->PSSetShaderResources(0, 0, nullptr);
//      _deviceContext->OMSetRenderTargets(0, nullptr, _depthTarget);
//
//      // clear
//      _deviceContext->ClearDepthStencilView(_depthTarget, D3D11_CLEAR_DEPTH,
//                                            1.0f, 0);
//
//      // set shaders
//      //_deviceContext->VSSetShader(interpolatedVertexShader, nullptr, 0);
//      _deviceContext->PSSetShader(discardPixelShader, nullptr, 0);
//
//      // update buffers
//      D3D11_MAPPED_SUBRESOURCE mappedResource;
//      _deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
//                          &mappedResource);
//      memcpy(mappedResource.pData, &worldMatrix, sizeof(glm::mat4));
//      _deviceContext->Unmap(_matrixBuffer, 0);
//      _deviceContext->Map(_showDepthBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
//                          &mappedResource);
//      memcpy(mappedResource.pData, &showDepth, sizeof(bool));
//      _deviceContext->Unmap(_showDepthBuffer, 0);
//
//      // set buffers
//      _deviceContext->IASetInputLayout(inputLayout);
//      _deviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);
//
//      _deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
//
//      _deviceContext->IASetVertexBuffers(0, 2, buf, stride, offset);
//
//      // draw
//      _deviceContext->DrawIndexedInstanced(6, 4, 0, 0, 0);
//
//      // second pass
//      // set render target
//      _deviceContext->OMSetRenderTargets(1, &_backBuffer, nullptr);
//
//      float clear[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
//      // clear
//      _deviceContext->ClearRenderTargetView(_backBuffer, clear);
//
//      // set shaders
//      //_deviceContext->VSSetShader(interpolatedVertexShader, nullptr, 0);
//      _deviceContext->PSSetShader(interpolatedPixelShader, nullptr, 0);
//
//      // update buffers
//      _deviceContext->Map(_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
//                          &mappedResource);
//      memcpy(mappedResource.pData, instanceData.data(), sizeof(glm::mat4) *
//      4);
//      _deviceContext->Unmap(_instanceBuffer, 0);
//
//      // set buffers
//      _deviceContext->IASetInputLayout(inputLayout);
//      _deviceContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);
//
//      _deviceContext->PSSetShaderResources(0, 1, &_depthMapTexture);
//      _deviceContext->PSSetConstantBuffers(0, 1, &_showDepthBuffer);
//
//      _deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
//
//      _deviceContext->IASetVertexBuffers(0, 2, buf, stride, offset);
//
//      // draw
//      _deviceContext->DrawIndexedInstanced(6, 4, 0, 0, 0);
//
//      _swapChain->Present(1, 0);
//    }
//
//    // shut down?
//  }
//
//  return EXIT_SUCCESS;
//}
//
// LRESULT CALLBACK DirectXGame::MessageHandler(HWND hWnd, UINT message,
//                                             WPARAM wParam, LPARAM lParam)
//{
//  switch (message)
//  {
//    case WM_DESTROY:
//    {
//      PostQuitMessage(0);
//      return 0;
//    }
//    // Check if the window is being closed.
//    case WM_CLOSE:
//    {
//      PostQuitMessage(0);
//      return 0;
//    }
//    case WM_KEYDOWN:
//    {
//      return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    case WM_KEYUP:
//    {
//      // for now, so that we can exit
//      if (wParam IS VK_ESCAPE)
//      {
//        PostQuitMessage(0);
//        return 0;
//      }
//      else
//      {
//        return DefWindowProc(hWnd, message, wParam, lParam);
//      }
//    }
//    default:
//    {
//      // forward the message to handler
//      return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//  }
//}

void DirectXGame::shutdownWindow()
{
  // adapted from rastertek.com
  // Show the mouse cursor.
  // ShowCursor(true);

  // Fix the display settings if leaving full screen mode.
  if (FULL_SCREEN)
  {
    ChangeDisplaySettings(NULL, 0);
  }

  // Remove the window.
  DestroyWindow(this->hwnd);
  this->hwnd = nullptr;

  // Remove the application instance.
  std::string windowTitle = this->getWindowTitle();
  std::wstring applicationName(windowTitle.begin(), windowTitle.end());
  UnregisterClass(applicationName.c_str(), this->hinstance);
  this->hinstance = nullptr;
}

void DirectXGame::shutdown()
{
  if (this->swapChain)
  {
    this->swapChain->SetFullscreenState(false, NULL);
  }

  SAFE_RELEASE(this->swapChain);
  this->shutdownWindow();
}

void DirectXGame::pollEvents()
{
  MSG msg;
  while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    if (msg.message == WM_QUIT)
    {
      this->quit();
    }
  }
}

} // namspace dx

} // namespace zge
#endif
