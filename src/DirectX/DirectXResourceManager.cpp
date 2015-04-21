#ifdef DIRECT_X
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "IGame.h"
#include "IShaderProgram.h"

#include "ShaderType.h"
#include "Shader.h"
#include "TextureType.h"
#include "DirectXResources.h"

#include "DirectXResourceManager.h"

#include "DirectXRenderer.h"
#include "ShaderManager.h"

namespace zge
{

IResourceManager* IResourceManager::getInstance()
{
  return dx::DirectXResourceManager::getInstance();
}

namespace dx
{

DirectXResourceManager* DirectXResourceManager::getInstance()
{
  static DirectXResourceManager instance;
  return &instance;
}

DirectXResourceManager::DirectXResourceManager()
    // ensure the 0th element in the lists are invalid (null)
    : buffers(1, nullptr),
      meshes(1, {}),
      computeShaders(1, nullptr),
      vertexShaders(1, nullptr),
      geometryShaders(1, nullptr),
      hullShaders(1, nullptr),
      domainShaders(1, nullptr),
      pixelShaders(1, nullptr),
      inputLayouts(1, nullptr),
      shaderPrograms(1, {}),
      vertexShaderByteCode(1, { nullptr, 0 }),
      shaderTextures(1, { nullptr, nullptr }),
      outputTargets(1, {})
{
  // constructor
}

DirectXResourceManager::~DirectXResourceManager() NOEXCEPT
{
  // destructor
  for (std::vector<ID3D11Buffer*>::iterator it = this->buffers.begin();
       it IS_NOT this->buffers.end(); ++it)
  {
    SAFE_RELEASE((*it));
  }

  for (std::vector<ID3D11ComputeShader*>::iterator it =
           this->computeShaders.begin();
       it IS_NOT this->computeShaders.end(); ++it)
  {
    SAFE_RELEASE((*it));
  }

  for (std::vector<ID3D11VertexShader*>::iterator it =
           this->vertexShaders.begin();
       it IS_NOT this->vertexShaders.end(); ++it)
  {
    SAFE_RELEASE((*it));
  }

  for (std::vector<ID3D11GeometryShader*>::iterator it =
           this->geometryShaders.begin();
       it IS_NOT this->geometryShaders.end(); ++it)
  {
    SAFE_RELEASE((*it));
  }

  for (std::vector<ID3D11HullShader*>::iterator it = this->hullShaders.begin();
       it IS_NOT this->hullShaders.end(); ++it)
  {
    SAFE_RELEASE((*it));
  }

  for (std::vector<ID3D11DomainShader*>::iterator it =
           this->domainShaders.begin();
       it IS_NOT this->domainShaders.end(); ++it)
  {
    SAFE_RELEASE((*it));
  }

  for (std::vector<ID3D11PixelShader*>::iterator it =
           this->pixelShaders.begin();
       it IS_NOT this->pixelShaders.end(); ++it)
  {
    SAFE_RELEASE((*it));
  }

  for (std::vector<ID3D11InputLayout*>::iterator it =
           this->inputLayouts.begin();
       it IS_NOT this->inputLayouts.end(); ++it)
  {
    SAFE_RELEASE((*it));
  }

  for (std::vector<std::pair<ID3D11Texture2D*,
                             ID3D11ShaderResourceView*> >::iterator it =
           this->shaderTextures.begin();
       it IS_NOT this->shaderTextures.end(); ++it)
  {
    SAFE_RELEASE(it->first);
    SAFE_RELEASE(it->second);
  }

  for (std::vector<Output>::iterator it = this->outputTargets.begin();
       it IS_NOT this->outputTargets.end(); ++it)
  {
    SAFE_RELEASE(it->depthTarget);
    for (std::vector<ID3D11RenderTargetView*>::iterator i =
             it->outputTargets.begin();
         i IS_NOT it->outputTargets.end(); ++i)
    {
      SAFE_RELEASE((*i));
    }
  }

  for (int i = 0; i < 3; ++i)
  {
    SAFE_RELEASE(this->depthStencilStates[i]);
  }

  for (int i = 0; i < 2; ++i)
  {
    SAFE_RELEASE(this->blendStates[i]);
  }

  for (int i = 0; i < TOTAL_RASTER_STATES; i++)
  {
    SAFE_RELEASE(this->rasterStates[i]);
  }

  for (int i = 0; i < TOTAL_SAMPLERS; i++)
  {
    SAFE_RELEASE(this->samplers[i]);
  }

  SAFE_RELEASE(this->device);
}

DirectXResourceManager::DirectXResourceManager(
    const DirectXResourceManager& other)
{
  // copy constructor
}

DirectXResourceManager& DirectXResourceManager::operator=(
    const DirectXResourceManager& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

DirectXResourceManager::DirectXResourceManager(
    DirectXResourceManager&& other) NOEXCEPT
{
  // move constructor (C++11)
}

DirectXResourceManager& DirectXResourceManager::operator=(
    DirectXResourceManager&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new DirectXResourceManager(other));
}

const bool DirectXResourceManager::initialise()
{
  // create depth states
  this->depthStencilStates[ENABLED] = this->createDepthEnabled();
  if (this->depthStencilStates[ENABLED] IS nullptr)
  {
    return false;
  }
  this->depthStencilStates[DISABLED] = this->createDepthDisabled();
  if (this->depthStencilStates[DISABLED] IS nullptr)
  {
    return false;
  }

  // create alpha states TODO: alpha states
  // create rasterizer states TODO: raterizer states
  // D3D11_RASTERIZER_DESC rasterDesc;
  // ZeroMemory(&rasterDesc, sizeof(rasterDesc));
  // rasterDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
  // rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
  // ID3D11RasterizerState* pResult;
  // if (FAILED(this->device->CreateRasterizerState(&rasterDesc, &pResult)))
  //{
  //  return false;
  //}
  // ID3D11DeviceContext* context;
  // this->device->GetImmediateContext(&context);
  // context->RSSetState(pResult);

  this->rasterStates[CULL_OFF] = this->createRasterState(D3D11_CULL_NONE, D3D11_FILL_SOLID);
  if (this->rasterStates[CULL_OFF] IS nullptr)
  {
    return false;
  }

  this->rasterStates[CULL_FRONT] = this->createRasterState(D3D11_CULL_BACK, D3D11_FILL_SOLID);
  if (this->rasterStates[CULL_FRONT] IS nullptr)
  {
    return false;
  }

  this->rasterStates[CULL_BACK] = this->createRasterState(D3D11_CULL_FRONT, D3D11_FILL_SOLID);
  if (this->rasterStates[CULL_BACK] IS nullptr)
  {
    return false;
  }

  this->rasterStates[WIREFRAME] = this->createRasterState(D3D11_CULL_FRONT, D3D11_FILL_WIREFRAME);
  if (this->rasterStates[WIREFRAME] IS nullptr)
  {
    return false;
  }

  // create samplers and set them
  // based on
  // https://directxtk.codeplex.com/SourceControl/latest#Src/CommonStates.cpp
  this->samplers[POINT_WRAP] = this->createSampler(
      D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP);
  if (this->samplers[POINT_WRAP] IS nullptr)
  {
    return false;
  }

  this->samplers[POINT_CLAMP] = this->createSampler(
      D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP);
  if (this->samplers[POINT_CLAMP] IS nullptr)
  {
    return false;
  }

  this->samplers[LINEAR_WRAP] = this->createSampler(
      D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
  if (this->samplers[LINEAR_WRAP] IS nullptr)
  {
    return false;
  }

  this->samplers[LINEAR_CLAMP] = this->createSampler(
      D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);
  if (this->samplers[LINEAR_CLAMP] IS nullptr)
  {
    return false;
  }

  DirectXRenderer::getInstance()->setSamplers(this->samplers);

  // create the default layout
  ShaderManager* shaderManager = ShaderManager::getInstance();
  for (int i = SHADER_NULL + 1; i < SHADER_TOTAL; ++i)
  {
    IShaderProgram* program = shaderManager->getShader(i);
    unsigned programId = program->getProgramId();
    // TODO: actually create an input layout based on the attributes instead of
    // just using the default one

    this->createInputLayout(programId, this->getBasicLayoutDescription());
  }

  return true;
}

const unsigned DirectXResourceManager::createMesh(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec3>& normals,
    const std::vector<glm::uvec3>& indices)
{
  if (vertices.size() IS_NOT normals.size())
  {
    std::cerr
        << "Error creating mesh, not the same amount of vertices and normals"
        << std::endl;
    return 0;
  }

  const D3D11_BUFFER_DESC vertexBufferDescription =
      this->getVertexBufferDescription(sizeof(glm::vec3) * vertices.size());

  ID3D11Buffer* vertexBuffer =
      this->createBuffer(vertices.data(), vertexBufferDescription);
  if (vertexBuffer IS nullptr)
  {
    return 0;
  }

  ID3D11Buffer* indexBuffer = this->createBuffer(
      indices.data(),
      this->getIndexBufferDescription(sizeof(glm::uvec3) * indices.size()));

  if (indexBuffer IS nullptr)
  {
    vertexBuffer->Release();
    return 0;
  }

  ID3D11Buffer* normalBuffer =
      this->createBuffer(normals.data(), vertexBufferDescription);

  if (normalBuffer IS nullptr)
  {
    vertexBuffer->Release();
    indexBuffer->Release();
    return 0;
  }

  this->buffers.push_back(vertexBuffer);
  unsigned vertexBufferId = this->buffers.size() - 1;

  this->buffers.push_back(indexBuffer);
  unsigned indexBufferId = this->buffers.size() - 1;

  this->buffers.push_back(normalBuffer);
  unsigned normalBufferId = this->buffers.size() - 1;

  Mesh mesh = { indices.size() * 3, vertexBufferId, indexBufferId,
                normalBufferId };

  this->meshes.push_back(mesh);

  return this->meshes.size() - 1;
}

const unsigned DirectXResourceManager::createMesh(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec3>& normals,
    const std::vector<glm::uvec3>& indices,
    const std::vector<glm::vec2>& textureCoordinates,
    const std::vector<glm::vec3>& tangents,
    const std::vector<glm::vec3>& bitangents)
{
  // TODO: implement createMesh with tangents
  return 0;
}

const unsigned DirectXResourceManager::getInstanceBuffer()
{
  unsigned instanceBufferId = ResourceManager::getInstanceBuffer();

  if (instanceBufferId IS 0)
  {
    return this->createInstanceBuffer(1);
  }

  return instanceBufferId;
}

const unsigned DirectXResourceManager::compileShaderCode(
    const void* shaderCode, const size_t shaderCodeSize,
    const unsigned shaderType)
{
  // returns 0 if not exists
  unsigned shaderId =
      this->shaderExists(shaderCode, shaderCodeSize, shaderType);
  if (shaderCode IS_NOT nullptr and shaderId IS 0)
  {
    switch (shaderType)
    {
      case zge::COMPUTE_SHADER:
      {
        ID3D11ComputeShader* computeShader;
        if (FAILED(this->device->CreateComputeShader(shaderCode, shaderCodeSize,
                                                     NULL, &computeShader)))
        {
          return 0;
        }

        this->computeShaders.push_back(computeShader);
        shaderId = this->computeShaders.size() - 1;
        break;
      }
      case zge::VERTEX_SHADER:
      {
        ID3D11VertexShader* vertexShader;
        if (FAILED(this->device->CreateVertexShader(shaderCode, shaderCodeSize,
                                                    NULL, &vertexShader)))
        {
          return 0;
        }

        // MAYBE: save the byteCode and size for validating the input layout
        // created later
        this->vertexShaders.push_back(vertexShader);
        this->vertexShaderByteCode.push_back(
            std::make_pair(shaderCode, shaderCodeSize));
        shaderId = this->vertexShaders.size() - 1;
        break;
      }
      case zge::GEOMETRY_SHADER:
      {
        ID3D11GeometryShader* geometryShader;
        if (FAILED(this->device->CreateGeometryShader(
                shaderCode, shaderCodeSize, NULL, &geometryShader)))
        {
          return 0;
        }

        this->geometryShaders.push_back(geometryShader);
        shaderId = this->geometryShaders.size() - 1;
        break;
      }
      case zge::HULL_SHADER:
      {
        ID3D11HullShader* hullShader;
        if (FAILED(this->device->CreateHullShader(shaderCode, shaderCodeSize,
                                                  NULL, &hullShader)))
        {
          return 0;
        }

        this->hullShaders.push_back(hullShader);
        shaderId = this->hullShaders.size() - 1;
        break;
      }
      case zge::DOMAIN_SHADER:
      {
        ID3D11DomainShader* domainShader;
        if (FAILED(this->device->CreateDomainShader(shaderCode, shaderCodeSize,
                                                    NULL, &domainShader)))
        {
          return 0;
        }

        this->domainShaders.push_back(domainShader);
        shaderId = this->domainShaders.size() - 1;
        break;
      }
      case zge::PIXEL_SHADER:
      {
        ID3D11PixelShader* pixelShader;
        if (FAILED(this->device->CreatePixelShader(shaderCode, shaderCodeSize,
                                                   NULL, &pixelShader)))
        {
          return 0;
        }

        this->pixelShaders.push_back(pixelShader);
        shaderId = this->pixelShaders.size() - 1;
        break;
      }
      default:
      {
        return 0;
      }
    }

    this->addShader(shaderCode, shaderCodeSize, shaderType, shaderId);
  }

  return shaderId;
}

const unsigned DirectXResourceManager::createShaderProgram(
    const unsigned computeShaderId, const unsigned vertexShaderId,
    const unsigned geometryShaderId, const unsigned hullShaderId,
    const unsigned domainShaderId, const unsigned pixelShaderId)
{
  unsigned programId = this->shaderProgramExists(
      computeShaderId, vertexShaderId, geometryShaderId, hullShaderId,
      domainShaderId, pixelShaderId);
  if (programId IS 0 and (computeShaderId IS_NOT 0 or vertexShaderId IS_NOT 0 or
                          geometryShaderId IS_NOT 0 or hullShaderId IS_NOT 0 or
                          domainShaderId IS_NOT 0 or pixelShaderId IS_NOT 0))
  {
    Program program{ computeShaderId, vertexShaderId, geometryShaderId,
                     hullShaderId,    domainShaderId, pixelShaderId };

    this->shaderPrograms.push_back(program);
    programId = this->shaderPrograms.size() - 1;
  }

  return programId;
}

const unsigned DirectXResourceManager::createRawTexture2D(
    const unsigned width, const unsigned height, const unsigned textureType)
{
  D3D11_TEXTURE2D_DESC textureDescription;
  D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescription;
  switch (textureType)
  {
    case zge::ZGE_DEPTH:
    {
      textureDescription = this->getDepthTextureDescription(width, height);
      shaderResourceViewDescription =
          this->getShaderResourceViewDescription(DXGI_FORMAT_R32_FLOAT);
      break;
    }
    case zge::ZGE_COLOR:
    {
      textureDescription = this->getColorTextureDescription(width, height);
      shaderResourceViewDescription = this->getShaderResourceViewDescription(
          DXGI_FORMAT_R32G32B32A32_FLOAT);
      break;
    }
    default:
    {
      return 0;
    }
  }

  // create the texture2d
  ID3D11Texture2D* texture;
  if (FAILED(device->CreateTexture2D(&textureDescription, NULL, &texture)))
  {
    return 0;
  }
  // create the shader resource view
  ID3D11ShaderResourceView* shaderResourceView;
  if (FAILED(device->CreateShaderResourceView(
          texture, &shaderResourceViewDescription, &shaderResourceView)))
  {
    // release the texture also
    texture->Release();
    return 0;
  }

  this->shaderTextures.push_back({ texture, shaderResourceView });

  return this->shaderTextures.size() - 1;
}

const unsigned DirectXResourceManager::createOutputBuffer(
    const unsigned depthTexture, const std::vector<unsigned>& outputTextures)
{
  ID3D11DepthStencilView* depthTarget;
  if (depthTexture IS_NOT 0)
  {
    // create depth stencil view
    if (FAILED(this->device->CreateDepthStencilView(
            this->shaderTextures[depthTexture].first,
            &this->getDepthStencilViewDescription(), &depthTarget)))
    {
      return 0;
    }
  }

  std::vector<ID3D11RenderTargetView*> renderTargetViews;
  D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescription =
      this->getRenderTargetViewDescription();
  // create render target views foreach outputTexture
  for (int i = 0; i < outputTextures.size(); ++i)
  {
    ID3D11RenderTargetView* renderTargetView;
    if (FAILED(this->device->CreateRenderTargetView(
            this->shaderTextures[outputTextures[i]].first,
            &renderTargetViewDescription, &renderTargetView)))
    {
      // release the stencil view
      depthTarget->Release();
      // release the previous render targets
      for (int i = 0; i < renderTargetViews.size(); ++i)
      {
        renderTargetViews[i]->Release();
      }
      return 0;
    }

    renderTargetViews.push_back(renderTargetView);
  }

  this->outputTargets.push_back({ depthTarget, renderTargetViews });

  return this->outputTargets.size() - 1;
}

const unsigned DirectXResourceManager::createInstanceBuffer(
    const unsigned long dataSize)
{
  ID3D11Buffer* buffer =
      this->createBuffer(nullptr, this->getInstanceBufferDescription(dataSize));

  if (buffer IS nullptr)
  {
    return 0;
  }

  unsigned instanceBufferId = ResourceManager::getInstanceBuffer();
  if (instanceBufferId IS_NOT 0)
  {
    // one exists, so delete the old one and replace
    ID3D11Buffer* old = this->buffers[instanceBufferId];
    old->Release();
    old = nullptr;

    this->buffers[instanceBufferId] = buffer;
  }
  else
  {
    this->buffers.push_back(buffer);
    instanceBufferId = this->buffers.size() - 1;

    ResourceManager::setInstanceBuffer(instanceBufferId);
  }

  return instanceBufferId;
}

const unsigned DirectXResourceManager::createInputLayout(
    const unsigned programId,
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDescription)
{
  unsigned vertexShaderId = this->shaderPrograms[programId].vertexShader;
  std::pair<const void*, std::size_t> byteCode =
      this->vertexShaderByteCode[vertexShaderId];

  ID3D11InputLayout* inputLayout;
  if (FAILED(this->device->CreateInputLayout(
          inputLayoutDescription.data(), inputLayoutDescription.size(),
          byteCode.first, byteCode.second, &inputLayout)))
  {
    return 0;
  }

  this->inputLayouts.push_back(inputLayout);
  unsigned inputLayoutId = this->inputLayouts.size() - 1;

  Program program = this->shaderPrograms[programId];

  program.inputLayout = inputLayoutId;

  this->shaderPrograms[programId] = program;

  return inputLayoutId;
}

std::vector<unsigned>* DirectXResourceManager::getUniformBuffers(
    const unsigned programId)
{
  return &this->uniformBuffers[programId];
}

unsigned DirectXResourceManager::createUniformBuffer(
    const unsigned long bufferSize)
{
  unsigned multipleOfSixteenSize;
  if (bufferSize % 16 IS 0)
  {
    multipleOfSixteenSize = bufferSize;
  }
  else
  {
    multipleOfSixteenSize = bufferSize / 16 + 16;
  }
  ID3D11Buffer* buffer = this->createBuffer(
      nullptr, this->getConstantBufferDescription(multipleOfSixteenSize));

  this->buffers.push_back(buffer);

  return this->buffers.size() - 1;
}

ID3D11Buffer* DirectXResourceManager::createBuffer(
    const void* data, const D3D11_BUFFER_DESC& bufferDescription)
{
  D3D11_SUBRESOURCE_DATA bufferData;
  ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
  if (data IS nullptr)
  {
    bufferData = { new int, 0, 0 };
  }
  else
  {
    bufferData = { data, 0, 0 };
  }
  ID3D11Buffer* buffer;

  if (FAILED(
          this->device->CreateBuffer(&bufferDescription, &bufferData, &buffer)))
  {
    return 0;
  }

  return buffer;
}

const D3D11_BUFFER_DESC DirectXResourceManager::getInstanceBufferDescription(
    const unsigned long& dataSize) const
{
  D3D11_BUFFER_DESC instanceBufferDescription;
  instanceBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
  instanceBufferDescription.ByteWidth = dataSize;
  instanceBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  instanceBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  instanceBufferDescription.MiscFlags = 0;
  instanceBufferDescription.StructureByteStride = 0;

  return instanceBufferDescription;
}

const D3D11_BUFFER_DESC DirectXResourceManager::getVertexBufferDescription(
    const unsigned long& dataSize) const
{
  D3D11_BUFFER_DESC vertexBufferDescription;
  vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDescription.ByteWidth = dataSize;
  vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDescription.CPUAccessFlags = 0;
  vertexBufferDescription.MiscFlags = 0;
  vertexBufferDescription.StructureByteStride = 0;

  return vertexBufferDescription;
}

const D3D11_BUFFER_DESC DirectXResourceManager::getIndexBufferDescription(
    const unsigned long& dataSize) const
{
  D3D11_BUFFER_DESC indexBufferDescription;
  indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDescription.ByteWidth = dataSize;
  indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDescription.CPUAccessFlags = 0;
  indexBufferDescription.MiscFlags = 0;
  indexBufferDescription.StructureByteStride = 0;

  return indexBufferDescription;
}

const D3D11_BUFFER_DESC DirectXResourceManager::getConstantBufferDescription(
    const unsigned long& dataSize) const
{
  D3D11_BUFFER_DESC constantBufferDescription;
  constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
  constantBufferDescription.ByteWidth = dataSize;
  constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  constantBufferDescription.MiscFlags = 0;
  constantBufferDescription.StructureByteStride = 0;

  return constantBufferDescription;
}

const D3D11_TEXTURE2D_DESC DirectXResourceManager::getColorTextureDescription(
    const unsigned width, const unsigned height) const
{
  D3D11_TEXTURE2D_DESC colorTextureDescription;
  colorTextureDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  colorTextureDescription.Width = width;
  colorTextureDescription.Height = height;
  colorTextureDescription.ArraySize = 1;
  colorTextureDescription.MipLevels = 1;
  colorTextureDescription.BindFlags =
      D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
  colorTextureDescription.Usage = D3D11_USAGE_DEFAULT;
  colorTextureDescription.CPUAccessFlags = 0;
  colorTextureDescription.SampleDesc.Count = 1;
  colorTextureDescription.SampleDesc.Quality = 0;
  colorTextureDescription.MiscFlags = 0;

  return colorTextureDescription;
}

const D3D11_TEXTURE2D_DESC DirectXResourceManager::getDepthTextureDescription(
    const unsigned width, const unsigned height) const
{
  D3D11_TEXTURE2D_DESC depthTextureDescription;
  depthTextureDescription.Format = DXGI_FORMAT_R32_TYPELESS;
  depthTextureDescription.Width = width;
  depthTextureDescription.Height = height;
  depthTextureDescription.ArraySize = 1;
  depthTextureDescription.MipLevels = 1;
  depthTextureDescription.BindFlags =
      D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  depthTextureDescription.Usage = D3D11_USAGE_DEFAULT;
  depthTextureDescription.CPUAccessFlags = 0;
  depthTextureDescription.SampleDesc.Count = 1;
  depthTextureDescription.SampleDesc.Quality = 0;
  depthTextureDescription.MiscFlags = 0;

  return depthTextureDescription;
}

const D3D11_DEPTH_STENCIL_VIEW_DESC
DirectXResourceManager::getDepthStencilViewDescription() const
{
  D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescription;
  depthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  depthStencilViewDescription.Format = DXGI_FORMAT_D32_FLOAT;
  depthStencilViewDescription.Flags = 0;
  depthStencilViewDescription.Texture2D.MipSlice = 0;

  return depthStencilViewDescription;
}

const D3D11_RENDER_TARGET_VIEW_DESC
DirectXResourceManager::getRenderTargetViewDescription() const
{
  D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescription;
  renderTargetViewDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  renderTargetViewDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  renderTargetViewDescription.Texture2D.MipSlice = 0;

  return renderTargetViewDescription;
}

const D3D11_SHADER_RESOURCE_VIEW_DESC
DirectXResourceManager::getShaderResourceViewDescription(
    const DXGI_FORMAT format) const
{
  D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescription;
  shaderResourceViewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  shaderResourceViewDescription.Format = format;
  shaderResourceViewDescription.Texture2D.MostDetailedMip = 0;
  shaderResourceViewDescription.Texture2D.MipLevels = -1;

  return shaderResourceViewDescription;
}

std::vector<D3D11_INPUT_ELEMENT_DESC>
DirectXResourceManager::getLayoutDescription() const
{
  // TODO: input layout descs
  return {};
}

std::vector<D3D11_INPUT_ELEMENT_DESC>
DirectXResourceManager::getBasicLayoutDescription() const
{
  return {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
      D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0,
      D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "INSTANCE_MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0,
      D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "INSTANCE_MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2,
      D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "INSTANCE_MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2,
      D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "INSTANCE_MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2,
      D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
  };
}

const Program DirectXResourceManager::getProgram(const unsigned programId) const
{
  return this->shaderPrograms[programId];
}

const Output DirectXResourceManager::getOutput(const unsigned outputId) const
{
  return this->outputTargets[outputId];
}

const Mesh DirectXResourceManager::getMesh(const unsigned meshId) const
{
  return this->meshes[meshId];
}

ID3D11ComputeShader* DirectXResourceManager::getComputeShader(
    const unsigned shaderId) const
{
  return this->computeShaders[shaderId];
}

ID3D11VertexShader* DirectXResourceManager::getVertexShader(
    const unsigned shaderId) const
{
  return this->vertexShaders[shaderId];
}

ID3D11GeometryShader* DirectXResourceManager::getGeometryShader(
    const unsigned shaderId) const
{
  return this->geometryShaders[shaderId];
}

ID3D11HullShader* DirectXResourceManager::getHullShader(
    const unsigned shaderId) const
{
  return this->hullShaders[shaderId];
}

ID3D11DomainShader* DirectXResourceManager::getDomainShader(
    const unsigned shaderId) const
{
  return this->domainShaders[shaderId];
}

ID3D11PixelShader* DirectXResourceManager::getPixelShader(
    const unsigned shaderId) const
{
  return this->pixelShaders[shaderId];
}

ID3D11Buffer* DirectXResourceManager::getBuffer(const unsigned bufferId) const
{
  return this->buffers[bufferId];
}

ID3D11DepthStencilState* DirectXResourceManager::getDepthState(
    const unsigned depthState) const
{
  return this->depthStencilStates[depthState];
}

ID3D11RasterizerState* DirectXResourceManager::getRasterState(const unsigned rasterState) const
{
  return this->rasterStates[rasterState];
}

ID3D11ShaderResourceView* DirectXResourceManager::getTexture(
    const unsigned textureId) const
{
  return this->shaderTextures[textureId].second;
}

ID3D11InputLayout* DirectXResourceManager::getInputLayout(
    const unsigned inputLayoutId) const
{
  return this->inputLayouts[inputLayoutId];
}

ID3D11InputLayout* DirectXResourceManager::getBasicInputLayout() const
{
  return this->inputLayouts[0];
}

ID3D11DepthStencilState* DirectXResourceManager::createDepthDisabled() const
{
  // https://directxtk.codeplex.com/SourceControl/latest#Src/CommonStates.cpp
  D3D11_DEPTH_STENCIL_DESC depthStencilDescription;
  depthStencilDescription.DepthEnable = false;
  depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
  depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

  depthStencilDescription.StencilEnable = false;
  depthStencilDescription.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
  depthStencilDescription.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

  depthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  depthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

  depthStencilDescription.BackFace = depthStencilDescription.FrontFace;

  ID3D11DepthStencilState* depthStencilState;
  if (FAILED(this->device->CreateDepthStencilState(&depthStencilDescription,
                                                   &depthStencilState)))
  {
    return nullptr;
  }

  return depthStencilState;
}

ID3D11DepthStencilState* DirectXResourceManager::createDepthEnabled() const
{
  // https://directxtk.codeplex.com/SourceControl/latest#Src/CommonStates.cpp
  D3D11_DEPTH_STENCIL_DESC depthStencilDescription;
  depthStencilDescription.DepthEnable = true;
  depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

  depthStencilDescription.StencilEnable = false;
  depthStencilDescription.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
  depthStencilDescription.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

  depthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  depthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

  depthStencilDescription.BackFace = depthStencilDescription.FrontFace;

  ID3D11DepthStencilState* depthStencilState;
  if (FAILED(this->device->CreateDepthStencilState(&depthStencilDescription,
                                                   &depthStencilState)))
  {
    return nullptr;
  }

  return depthStencilState;
}

ID3D11SamplerState* DirectXResourceManager::createSampler(
    D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode) const
{
  // https://directxtk.codeplex.com/SourceControl/latest#Src/CommonStates.cpp
  D3D11_SAMPLER_DESC samplerDescription;
  ZeroMemory(&samplerDescription, sizeof(samplerDescription));

  samplerDescription.Filter = filter;

  samplerDescription.AddressU = addressMode;
  samplerDescription.AddressV = addressMode;
  samplerDescription.AddressW = addressMode;

  samplerDescription.MaxAnisotropy =
      (this->device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? 16 : 2;

  samplerDescription.MaxLOD = FLT_MAX;
  samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;

  ID3D11SamplerState* sampler;

  if (FAILED(this->device->CreateSamplerState(&samplerDescription, &sampler)))
  {
    return nullptr;
  }

  return sampler;
}

ID3D11RasterizerState* DirectXResourceManager::createRasterState(
    D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode) const
{
  ID3D11RasterizerState* result;

  D3D11_RASTERIZER_DESC rasterizerDescription;
  ZeroMemory(&rasterizerDescription, sizeof(rasterizerDescription));

  rasterizerDescription.CullMode = cullMode;
  rasterizerDescription.FillMode = fillMode;
  rasterizerDescription.DepthClipEnable = true;
  rasterizerDescription.MultisampleEnable = true;

  if (FAILED(device->CreateRasterizerState(&rasterizerDescription, &result)))
  {
    return nullptr;
  }

  return result;
}

void DirectXResourceManager::setDevice(ID3D11Device* device)
{
  this->device = device;
}

bool DirectXResourceManager::setBackBufferTexture(
    ID3D11Texture2D* backBufferTexture)
{
  assert(this->outputTargets.size() IS 1);
  if (backBufferTexture IS_NOT nullptr)
  {
    Output backBufferOutput;

    ID3D11RenderTargetView* renderTargetView;
    if (FAILED(this->device->CreateRenderTargetView(backBufferTexture, nullptr,
                                                    &renderTargetView)))
    {
      return false;
    }

#if defined(_DEBUG)
    const char str_backBuffer[] = "BackBuffer RenderTargetView";
    renderTargetView->SetPrivateData(
        WKPDID_D3DDebugObjectName, sizeof(str_backBuffer) - 1, str_backBuffer);
#endif
    backBufferOutput.depthTarget = nullptr;
    backBufferOutput.outputTargets.push_back(renderTargetView);

    this->outputTargets.push_back(backBufferOutput);

    // assert that it has an Id of 1
    assert(this->outputTargets[1].depthTarget IS backBufferOutput.depthTarget);
    assert(this->outputTargets[1]
               .outputTargets[0] IS backBufferOutput.outputTargets[0]);

    SAFE_RELEASE(backBufferTexture);

    return true;
  }
  else
  {
    std::cerr << "ERROR: cannot set the back buffer to nullptr..." << std::endl;
    return false;
  }
}

const glm::mat4 DirectXResourceManager::getIdentityMatrix() const
{
  return glm::scale(glm::mat4(), glm::vec3{ 1.0f, 1.0f, -1.0f });
}

} // namespace dx

} // namespace zge

#endif
