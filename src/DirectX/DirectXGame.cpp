#ifdef DIRECT_X

#include <glm/gtx/transform.hpp>

#include "DirectXGame.h"
#include "IGame.h"
#include "DirectXShader.h"
#include "InterpolatedVS.h"
#include "DiscardPS.h"
#include "InterpolatedDepthPS.h"

namespace zge
{
namespace dx
{

bool DirectXGame::showDepth = false;

DirectXGame::DirectXGame(std::string applicationName)
{
  // constructor
	m_applicationName = applicationName;
}

DirectXGame::~DirectXGame() NOEXCEPT
{
  // destructor
  SAFE_RELEASE(m_instanceBuffer);
  SAFE_RELEASE(m_indexBuffer);
  SAFE_RELEASE(m_vertexBuffer);
  SAFE_RELEASE(m_sampler);
  SAFE_RELEASE(m_backBuffer);
  SAFE_RELEASE(m_depthTarget);
  SAFE_RELEASE(m_depthMapTexture);
  //SAFE_RELEASE(m_depthOff);
  SAFE_RELEASE(m_depthDefault);
  SAFE_RELEASE(m_deviceContext);
  SAFE_RELEASE(m_device);
  SAFE_RELEASE(m_swapChain);

  shutdownWindow();
}

DirectXGame::DirectXGame(const DirectXGame& other)
{
  // copy constructor
}

DirectXGame& DirectXGame::operator=(const DirectXGame& rhs)
{
  // assignement operator
  if (this IS &rhs)
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

const bool DirectXGame::initialiseWindow()
{
  // TODO: Error handling and return false.
  // adapted from rastertek.com
  int width, height;
  WNDCLASSEX windowClass;
  DEVMODE screenSettings;
  int posX, posY;

  m_hinstance = GetModuleHandle(NULL);

  std::wstring stemp = std::wstring(m_applicationName.begin(), m_applicationName.end());

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
  windowClass.lpszClassName = m_applicationName.c_str();
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
  m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName.c_str(), m_applicationName.c_str(),
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
    if (displayModeList[i].Width IS (unsigned int)SCREEN_WIDTH())
    {
      if (displayModeList[i].Height IS (unsigned int)SCREEN_HEIGHT())
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

  bool vsyncEnabled = true;
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
          D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL,
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

  if (FAILED(m_device->CreateRenderTargetView(texture2d[0], NULL, &m_backBuffer)))
  {
    return false;
  }

#if defined(_DEBUG)
  const char str_backBuffer[] = "BackBuffer";
  m_backBuffer->SetPrivateData(WKPDID_D3DDebugObjectName,
                              sizeof(str_backBuffer) - 1, str_backBuffer);
#endif

  D3D11_TEXTURE2D_DESC textureDescription;
  ZeroMemory(&textureDescription, sizeof(textureDescription));
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

  if (FAILED(m_device->CreateDepthStencilState(&desc, &m_depthDefault)))
  {
    return false;
  }

  //desc.DepthEnable = false;
  //desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
  //if (FAILED(m_device->CreateDepthStencilState(&desc, &m_depthOff)))
  //{
  //  return false;
  //}

#if defined(_DEBUG)
  const char str_Default[] = "DepthState Default";
  m_depthDefault->SetPrivateData(WKPDID_D3DDebugObjectName,
                                 sizeof(str_Default) - 1, str_Default);

  //const char strOffr[] = "DepthState Off";
  //m_depthOff->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(strOffr) - 1,
  //                           strOffr);
#endif

  m_deviceContext->OMSetDepthStencilState(m_depthDefault, 1);

  D3D11_VIEWPORT viewport = CD3D11_VIEWPORT(0.0f, 0.0f, (float)SCREEN_WIDTH(),
                                            (float)SCREEN_HEIGHT());

  m_deviceContext->RSSetViewports(1, &viewport);

  D3D11_SAMPLER_DESC samplerDescription;

  samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
  samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDescription.MipLODBias = 0.0f;
  samplerDescription.MaxAnisotropy = 1;
  samplerDescription.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  samplerDescription.BorderColor[0] = 0;
  samplerDescription.BorderColor[1] = 0;
  samplerDescription.BorderColor[2] = 0;
  samplerDescription.BorderColor[3] = 0;
  samplerDescription.MinLOD = 0;
  samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;
  if (FAILED(m_device->CreateSamplerState(&samplerDescription, &m_sampler)))
  {
    return false;
  }

#ifdef _DEBUG
  const char str_samplers1[] = "PointClamp";
  m_sampler->SetPrivateData(WKPDID_D3DDebugObjectName,
                            sizeof(str_samplers1) - 1, str_samplers1);
#endif

  m_deviceContext->PSSetSamplers(0, 1, &m_sampler);

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

  if (FAILED(m_device->CreateShaderResourceView(texture2d[1], &depthTargetDesc,
                                              &m_depthMapTexture)))
  {
    return false;
  }

#ifdef _DEBUG
  const char str_depthTarget[] = "m_depthTarget";
  m_depthTarget->SetPrivateData(WKPDID_D3DDebugObjectName,
    sizeof(str_depthTarget) - 1, str_depthTarget);

  const char str_depthMapTexture[] = "m_depthMapTexture";
  m_depthMapTexture->SetPrivateData(WKPDID_D3DDebugObjectName,
    sizeof(str_depthMapTexture) - 1, str_depthMapTexture);
#endif

  

  //_buffers[Buffers::MATRIX_BUFFER] =
  //    DirectXResourceManager::getInstance()->getMatrixBuffer();
  //_buffers[Buffers::INVERSE_PROJECTION_BUFFER] =
  //    DirectXResourceManager::getInstance()->getInverseProjectionMatrixBuffer();
  //_buffers[Buffers::LIGHT_BUFFER] =
  //    DirectXResourceManager::getInstance()->getLightBuffer();
  //_buffers[Buffers::INSTANCE_BUFFER] =
  //    DirectXResourceManager::getInstance()->getInstanceBuffer();

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

const int DirectXGame::run()
{
  initialiseWindow();
  initialiseDirectX();

  // create shaders
  ID3D11VertexShader* interpolatedVertexShader;
  if (FAILED(m_device->CreateVertexShader(InterpolatedVS, InterpolatedVS_size, NULL, &interpolatedVertexShader)))
  {
    return EXIT_FAILURE;
  }

  ID3D11PixelShader* discardPixelShader;
  if (FAILED(m_device->CreatePixelShader(DiscardPS, DiscardPS_size, NULL, &discardPixelShader)))
  {
    return EXIT_FAILURE;
  }

  ID3D11PixelShader* interpolatedPixelShader;
  if (FAILED(m_device->CreatePixelShader(InterpolatedDepthPS, InterpolatedDepthPS_size, NULL, &interpolatedPixelShader)))
  {
    return EXIT_FAILURE;
  }

  // create input layout
  D3D11_INPUT_ELEMENT_DESC layoutDescription[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "WORLD_MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "WORLD_MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "WORLD_MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "WORLD_MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 } };

  ID3D11InputLayout* inputLayout;
  if (FAILED(m_device->CreateInputLayout(layoutDescription, 5,
    InterpolatedVS, InterpolatedVS_size, &inputLayout)))
  {
    return EXIT_FAILURE;
  }

#if defined(_DEBUG)
  const char str_inputLayout[] = "InterpolatedVS inputLayout";
  inputLayout->SetPrivateData(WKPDID_D3DDebugObjectName,
    sizeof(str_inputLayout) - 1, str_inputLayout);
#endif

  // create buffers
  D3D11_BUFFER_DESC constantBufferDescription;

  constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
  constantBufferDescription.ByteWidth = sizeof(glm::mat4);
  constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  constantBufferDescription.MiscFlags = 0;
  constantBufferDescription.StructureByteStride = 0;

  if (FAILED(m_device->CreateBuffer(&constantBufferDescription, NULL, &m_matrixBuffer)))
  {
    return EXIT_FAILURE;
  }

#if defined(_DEBUG)
  const char str[] = "MatrixBuffer";
  m_matrixBuffer->SetPrivateData(WKPDID_D3DDebugObjectName,
    sizeof(str) - 1, str);
#endif

  constantBufferDescription.ByteWidth = 16;

  if (FAILED(m_device->CreateBuffer(&constantBufferDescription, NULL, &m_showDepthBuffer)))
  {
    return EXIT_FAILURE;
  }

#if defined(_DEBUG)
  const char ShowDepthBufferstr[] = "ShowDepthBuffer";
  m_showDepthBuffer->SetPrivateData(WKPDID_D3DDebugObjectName,
    sizeof(ShowDepthBufferstr) - 1, str);
#endif

  std::vector<glm::vec3> vertices;
  vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
  vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
  vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
  vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));

  m_vertexBuffer = createVertexBuffer(vertices);
  if (not m_vertexBuffer)
  {
    return EXIT_FAILURE;
  }
  m_indexBuffer = createIndexBuffer({ 0, 1, 2, 2, 3, 0 });
  m_instanceBuffer = createInstanceBuffer(4);

  std::vector<glm::vec3> instancePositions({
    glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
    glm::vec3(-0.5f, 0.5f, 0.5f),
  });

  // needed to set the vertex and instance buffers
  unsigned int stride[2];
  stride[0] = sizeof(glm::vec3);
  stride[1] = sizeof(glm::mat4);
  unsigned int offset[2];
  offset[0] = 0;
  offset[1] = 0;
  ID3D11Buffer* buf[2];
  buf[0] = m_vertexBuffer;
  buf[1] = m_instanceBuffer;

  m_deviceContext->VSSetShader(interpolatedVertexShader, nullptr, 0);
  m_deviceContext->PSSetShaderResources(0, 1, &m_depthMapTexture);

  // loop
  bool running = true;
  while (running)
  {
    MSG msg;
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    if (msg.message == WM_QUIT)
    {
      running = false;
    }
    else
    {
      std::vector<glm::mat4> instanceData(4);

      static float scale = 0.2f;
      glm::mat4 worldMatrix;
      worldMatrix = glm::scale(worldMatrix, glm::vec3(scale, scale, scale));

      static float translation = 0.0f;
      translation += 0.01f;
      for (int i = 0; i < 4; ++i)
      {
        glm::mat4 instDat;
        instDat = glm::translate(instDat, instancePositions[i]) *
          glm::translate(instDat, glm::vec3(sinf(translation) * 0.2f * 1.4f,
          cosf(translation) * 0.2f, 0.0f));
        instanceData[i] = glm::transpose(instDat);
      }

      // set render target
      m_deviceContext->PSSetShaderResources(0, 0, nullptr);
      m_deviceContext->OMSetRenderTargets(0, nullptr, m_depthTarget);

      // clear
      m_deviceContext->ClearDepthStencilView(m_depthTarget, D3D11_CLEAR_DEPTH, 1.0f, 0);

      // set shaders
      //m_deviceContext->VSSetShader(interpolatedVertexShader, nullptr, 0);
      m_deviceContext->PSSetShader(discardPixelShader, nullptr, 0);

      // update buffers
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      m_deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
      memcpy(mappedResource.pData, &worldMatrix, sizeof(glm::mat4));
      m_deviceContext->Unmap(m_matrixBuffer, 0);
      m_deviceContext->Map(m_showDepthBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
      memcpy(mappedResource.pData, &showDepth, sizeof(bool));
      m_deviceContext->Unmap(m_showDepthBuffer, 0);

      // set buffers
      m_deviceContext->IASetInputLayout(inputLayout);
      m_deviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

      m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

      m_deviceContext->IASetVertexBuffers(0, 2, buf, stride, offset);

      // draw
      m_deviceContext->DrawIndexedInstanced(6, 4, 0, 0, 0);


      // second pass
      // set render target
      m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, nullptr);

      float clear[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
      // clear
      m_deviceContext->ClearRenderTargetView(m_backBuffer, clear);

      // set shaders
      //m_deviceContext->VSSetShader(interpolatedVertexShader, nullptr, 0);
      m_deviceContext->PSSetShader(interpolatedPixelShader, nullptr, 0);

      // update buffers
      m_deviceContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
      memcpy(mappedResource.pData, instanceData.data(), sizeof(glm::mat4) * 4);
      m_deviceContext->Unmap(m_instanceBuffer, 0);

      // set buffers
      m_deviceContext->IASetInputLayout(inputLayout);
      m_deviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

      m_deviceContext->PSSetShaderResources(0, 1, &m_depthMapTexture);
      m_deviceContext->PSSetConstantBuffers(0, 1, &m_showDepthBuffer);

      m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

      m_deviceContext->IASetVertexBuffers(0, 2, buf, stride, offset);

      // draw
      m_deviceContext->DrawIndexedInstanced(6, 4, 0, 0, 0);

      m_swapChain->Present(1, 0);
    }

    // shut down?
  }

  return EXIT_SUCCESS;
}

LRESULT CALLBACK DirectXGame::MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
  case WM_DESTROY:
  {
    PostQuitMessage(0);
    return 0;
  }
  // Check if the window is being closed.
  case WM_CLOSE:
  {
    PostQuitMessage(0);
    return 0;
  }
  case WM_KEYDOWN:
  {
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  case WM_KEYUP:
  {
    // for now, so that we can exit
    if (wParam IS VK_ESCAPE)
    {
      PostQuitMessage(0);
      return 0;
    }
    else
    {
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
  default:
  {
    // forward the message to handler
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
	}
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
  UnregisterClass(m_applicationName.c_str(), m_hinstance);
  m_hinstance = NULL;
}

ID3D11Buffer* DirectXGame::createVertexBuffer(const std::vector<glm::vec3> vertices) const
{
  D3D11_BUFFER_DESC vertexBufferDesc;

  vertexBufferDesc.ByteWidth = vertices.size() * sizeof(glm::vec3);
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  D3D11_SUBRESOURCE_DATA vertexData;
  vertexData.pSysMem = vertices.data();
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;

  ID3D11Buffer* vertexBuffer;
  if (FAILED(m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer)))
  {
    return nullptr;
  }

#if defined(_DEBUG)
  const char str[] = "vertexBuffer";
  vertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName,
    sizeof(str) - 1, str);
#endif

  return vertexBuffer;
}

ID3D11Buffer* DirectXGame::createIndexBuffer(const std::vector<unsigned long> indices) const
{
  D3D11_BUFFER_DESC indexBufferDesc;

  indexBufferDesc.ByteWidth = indices.size() * sizeof(unsigned long);
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.CPUAccessFlags = 0;
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;

  D3D11_SUBRESOURCE_DATA indexData;
  indexData.pSysMem = indices.data();
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  ID3D11Buffer* indexBuffer;
  if (FAILED(m_device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer)))
  {
    return nullptr;
  }

#if defined(_DEBUG)
  const char str[] = "indexBuffer";
  indexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName,
    sizeof(str) - 1, str);
#endif

  return indexBuffer;
}

ID3D11Buffer* DirectXGame::createInstanceBuffer(const unsigned int size) const
{
  glm::mat4* tempData = new glm::mat4[size];

  D3D11_BUFFER_DESC instanceBufferDescription;
  instanceBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
  instanceBufferDescription.ByteWidth = sizeof(glm::mat4) * size;
  instanceBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  instanceBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  instanceBufferDescription.MiscFlags = 0;
  instanceBufferDescription.StructureByteStride = 0;

  D3D11_SUBRESOURCE_DATA instanceData;
  instanceData.pSysMem = tempData;
  instanceData.SysMemPitch = 0;
  instanceData.SysMemSlicePitch = 0;

  ID3D11Buffer* instanceBuffer;
  if (FAILED(m_device->CreateBuffer(&instanceBufferDescription, &instanceData, &instanceBuffer)))
  {
    return nullptr;
  }

#if defined(_DEBUG)
  const char str[] = "InstanceBuffer";
  instanceBuffer->SetPrivateData(WKPDID_D3DDebugObjectName,
    sizeof(str) - 1, str);
#endif

  return instanceBuffer;
}

} // namspace dx
} // namespace zge
#endif