#ifdef DIRECT_X
#include <iostream>

#include "IGame.h"
#include "IShaderProgram.h"
#include "Shader.h"
#include "DirectXResources.h"
#include "ShaderType.h"

#include "DirectXRenderer.h"

#include "DirectXResourceManager.h"
#include "ShaderManager.h"

namespace zge
{

IRenderer* IRenderer::getInstance()
{
  return dx::DirectXRenderer::getInstance();
}

namespace dx
{

DirectXRenderer* DirectXRenderer::getInstance()
{
  static DirectXRenderer instance;
  return &instance;
}

DirectXRenderer::DirectXRenderer()
{
  // constructor
}

DirectXRenderer::~DirectXRenderer() NOEXCEPT
{
  // destructor
}

DirectXRenderer::DirectXRenderer(const DirectXRenderer& other)
{
  // copy constructor
}

DirectXRenderer& DirectXRenderer::operator=(const DirectXRenderer& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

DirectXRenderer::DirectXRenderer(DirectXRenderer&& other) NOEXCEPT
{
  // move constructor (C++11)
}

DirectXRenderer& DirectXRenderer::operator=(DirectXRenderer&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new DirectXRenderer(other));
}

const bool DirectXRenderer::initialise()
{
  D3D11_VIEWPORT viewport;
  viewport.TopLeftX = 0.0f;
  viewport.TopLeftY = 0.0f;
  viewport.Width = (float)SCREEN_WIDTH();
  viewport.Height = (float)SCREEN_HEIGHT();
  viewport.MinDepth = D3D11_MIN_DEPTH;
  viewport.MaxDepth = D3D11_MAX_DEPTH;

  this->deviceContext->RSSetViewports(1, &viewport);

  this->deviceContext->IASetPrimitiveTopology(
      D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  return Renderer::initialise();
}

void DirectXRenderer::setShaderProgram(const unsigned programId)
{
  if (this->getShaderProgramId() IS_NOT programId)
  {
    DirectXResourceManager* resourceManager =
        DirectXResourceManager::getInstance();
    Program program = resourceManager->getProgram(programId);

    this->deviceContext->IASetInputLayout(
        resourceManager->getInputLayout(program.inputLayout));
    this->deviceContext->CSSetShader(
        resourceManager->getComputeShader(program.computeShader), nullptr, 0);
    this->deviceContext->VSSetShader(
        resourceManager->getVertexShader(program.vertexShader), nullptr, 0);
    this->deviceContext->GSSetShader(
        resourceManager->getGeometryShader(program.geometryShader), nullptr, 0);
    this->deviceContext->HSSetShader(
        resourceManager->getHullShader(program.hullShader), nullptr, 0);
    this->deviceContext->DSSetShader(
        resourceManager->getDomainShader(program.domainShader), nullptr, 0);
    this->deviceContext->PSSetShader(
        resourceManager->getPixelShader(program.pixelShader), nullptr, 0);

    Renderer::setShaderProgram(programId);
  }
}

void DirectXRenderer::setConstants(
    const void** bufferDataArray,
    const std::vector<unsigned long> bufferDataByteSizes,
    const unsigned shaderType)
{
  unsigned numberOfBuffers = bufferDataByteSizes.size();
  DirectXResourceManager* resourceManager =
      DirectXResourceManager::getInstance();
  std::vector<unsigned>* bufferIds =
      resourceManager->getUniformBuffers(this->getShaderProgramId());

  if (bufferIds->size() < numberOfBuffers)
  {
    for (int i = bufferIds->size() - 1; i < numberOfBuffers; ++i)
    {
      bufferIds->push_back(
          resourceManager->createUniformBuffer(bufferDataByteSizes[i]));
    }
  }

  std::vector<ID3D11Buffer*> buffers(numberOfBuffers);
  for (int i = 0; i < numberOfBuffers; ++i)
  {
    // update the buffer
    buffers[i] = resourceManager->getBuffer((*bufferIds)[i]);
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    deviceContext->Map(buffers[i], 0, D3D11_MAP_WRITE_DISCARD, 0,
                       &mappedResource);
    memcpy(mappedResource.pData, &bufferDataArray[i], bufferDataByteSizes[i]);
    deviceContext->Unmap(buffers[i], 0);
  }

  // bind the buffers
  this->bindConstantBuffers(shaderType, 0, buffers);
}

void DirectXRenderer::setConstant(const unsigned indexPosition,
                                  const void* bufferData,
                                  const unsigned long bufferDataByteSize,
                                  const unsigned shaderType)
{
  DirectXResourceManager* resourceManager =
      DirectXResourceManager::getInstance();

  std::vector<unsigned>* bufferIds =
      resourceManager->getUniformBuffers(this->getShaderProgramId());
  unsigned bufferId;
  if (bufferIds->size() < indexPosition + 1)
  {
    bufferIds->resize(indexPosition + 1);
    bufferId = resourceManager->createUniformBuffer(bufferDataByteSize);
    (*bufferIds)[indexPosition] = bufferId;
  }
  else
  {
    bufferId = (*bufferIds)[indexPosition];
    if (bufferId IS 0)
    {
      bufferId = resourceManager->createUniformBuffer(bufferDataByteSize);
      (*bufferIds)[indexPosition] = bufferId;
    }
  }

  ID3D11Buffer* buffer = resourceManager->getBuffer(bufferId);
  // update
  D3D11_MAPPED_SUBRESOURCE mappedResource;

  deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  memcpy(mappedResource.pData, bufferData, bufferDataByteSize);
  deviceContext->Unmap(buffer, 0);

  // bind the buffer
  this->bindConstantBuffers(shaderType, indexPosition,
                            std::vector<ID3D11Buffer*>{ buffer });
}

void DirectXRenderer::bindConstantBuffers(const unsigned shaderType,
                                          const unsigned indexPosition,
                                          std::vector<ID3D11Buffer*>& buffers)
{
  switch (shaderType)
  {
    case zge::COMPUTE_SHADER:
    {
      this->deviceContext->CSSetConstantBuffers(indexPosition, buffers.size(),
                                                buffers.data());
      break;
    }
    case zge::VERTEX_SHADER:
    {
      this->deviceContext->VSSetConstantBuffers(indexPosition, buffers.size(),
                                                buffers.data());
      break;
    }
    case zge::GEOMETRY_SHADER:
    {
      this->deviceContext->GSSetConstantBuffers(indexPosition, buffers.size(),
                                                buffers.data());
      break;
    }
    case zge::HULL_SHADER:
    {
      this->deviceContext->HSSetConstantBuffers(indexPosition, buffers.size(),
                                                buffers.data());
      break;
    }
    case zge::DOMAIN_SHADER:
    {
      this->deviceContext->DSSetConstantBuffers(indexPosition, buffers.size(),
                                                buffers.data());
      break;
    }
    case zge::PIXEL_SHADER:
    {
      this->deviceContext->PSSetConstantBuffers(indexPosition, buffers.size(),
                                                buffers.data());
      break;
    }
  }
}

void DirectXRenderer::executeInstancedShader(
    const unsigned meshId, const void* instanceDataArray,
    const unsigned long instanceDataUnitByteSize,
    const unsigned numberOfInstances)
{

  DirectXResourceManager* resourceManager =
      DirectXResourceManager::getInstance();

  unsigned bufferSize = instanceDataUnitByteSize * numberOfInstances;

  unsigned instanceBufferId;

  if (bufferSize > this->currentInstanceDataSize)
  {
    this->currentInstanceDataSize = bufferSize;

    instanceBufferId = resourceManager->createInstanceBuffer(bufferSize);
  }
  else
  {
    /*const unsigned */ instanceBufferId = resourceManager->getInstanceBuffer();
  }

  ID3D11Buffer* instanceBuffer = resourceManager->getBuffer(instanceBufferId);

  D3D11_MAPPED_SUBRESOURCE mappedResource;

  deviceContext->Map(instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
                     &mappedResource);
  memcpy(mappedResource.pData, instanceDataArray,
         instanceDataUnitByteSize * numberOfInstances);
  deviceContext->Unmap(instanceBuffer, 0);

  Program program = resourceManager->getProgram(this->getShaderProgramId());

  this->deviceContext->IASetInputLayout(
      resourceManager->getInputLayout(program.inputLayout));

  const Mesh mesh = resourceManager->getMesh(meshId);

  this->deviceContext->IASetIndexBuffer(
      resourceManager->getBuffer(mesh.indexBufferId), DXGI_FORMAT_R32_UINT, 0);

  ID3D11Buffer* vertexBuffers[] = {
    resourceManager->getBuffer(mesh.vertexBufferId),
    resourceManager->getBuffer(mesh.normalBufferId), instanceBuffer
  };

  unsigned int vec3size = sizeof(glm::vec3);
  unsigned int strides[] = { vec3size, vec3size, instanceDataUnitByteSize };
  unsigned int offsets[] = { 0, 0, 0 };

  this->deviceContext->IASetVertexBuffers(0, 3, vertexBuffers, strides,
                                          offsets);

  this->deviceContext->DrawIndexedInstanced(mesh.numberOfIndices,
                                            numberOfInstances, 0, 0, 0);
}

void DirectXRenderer::executeComputeShader()
{
  // TODO: implement executeComputeShader
}

void DirectXRenderer::executeShader()
{
  // TODO: implement executeShader
}

void DirectXRenderer::setTextures(const std::vector<unsigned> textureIds,
                                  const std::vector<unsigned> samplerTypes,
                                  const unsigned shaderType)
{
  unsigned numberOfTextures = textureIds.size();
  if (numberOfTextures IS_NOT samplerTypes.size())
  {
    std::cerr << "ERROR: DirectXRenderer::setTextures: Not same amount of "
                 "textureIds and samplers. Continuing..." << std::endl;
  }

  std::vector<ID3D11ShaderResourceView*> textures;

  for (int i = 0; i < numberOfTextures; i++)
  {
    textures.push_back(
        DirectXResourceManager::getInstance()->getTexture(textureIds[i]));
  }
  // FIXME: set textures not only in pixel shader
  this->deviceContext->PSSetShaderResources(0, numberOfTextures,
                                            textures.data());
}

void DirectXRenderer::setTexture(const unsigned textureLocation,
                                 const unsigned textureId,
                                 const unsigned samplerType,
                                 const unsigned shaderType)
{
  // FIXME: set textures not only in pixel shader
  ID3D11ShaderResourceView* texture =
      DirectXResourceManager::getInstance()->getTexture(textureId);
  this->deviceContext->PSSetShaderResources(textureLocation, 1, &texture);
}

void DirectXRenderer::setOutput(const unsigned outputId)
{
  unsigned Id; // = outputId;
  if (outputId IS 0)
  {
    // use back buffer (outputId = 1)
    Id = 1;
  }
  else
  {
    Id = outputId;
  }
  Output output = DirectXResourceManager::getInstance()->getOutput(Id);
  if (output.depthTarget IS_NOT nullptr)
  {
    this->deviceContext->ClearDepthStencilView(output.depthTarget,
                                               D3D11_CLEAR_DEPTH, 1.0f, 0);
  }
  unsigned numberOfOutputTargets = output.outputTargets.size();
  float clearColor[]{ CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b,
                      CLEAR_COLOR.a };
  for (int i = 0; i < numberOfOutputTargets; ++i)
  {
    this->deviceContext->ClearRenderTargetView(output.outputTargets[i],
                                               clearColor);
  }

  this->deviceContext->OMSetRenderTargets(
      numberOfOutputTargets, output.outputTargets.data(), output.depthTarget);
}

void DirectXRenderer::enableDepth() { this->setDepthState(ENABLED); }

void DirectXRenderer::disableDepth() { this->setDepthState(DISABLED); }

void DirectXRenderer::enableAntiClockwiseCulling()
{
  this->setRasterState(CULL_BACK);
}

void DirectXRenderer::disableCulling() { this->setRasterState(CULL_OFF); }

void DirectXRenderer::setDeviceContext(ID3D11DeviceContext* deviceContext)
{
  this->deviceContext = deviceContext;
}

void DirectXRenderer::setDepthState(const unsigned depthState)
{
  if (this->getDepthState() IS_NOT depthState)
  {
    this->deviceContext->OMSetDepthStencilState(
        DirectXResourceManager::getInstance()->getDepthState(depthState), 1);
    Renderer::setDepthState(depthState);
  }
}

void DirectXRenderer::setRasterState(const unsigned rasterState)
{
  if (this->getRasterState() IS_NOT rasterState)
  {
    this->deviceContext->RSSetState(
        DirectXResourceManager::getInstance()->getRasterState(rasterState));
    Renderer::setRasterState(rasterState);
  }
}

void DirectXRenderer::setSamplers(ID3D11SamplerState* samplers[]) const
{
  this->deviceContext->PSSetSamplers(0, TOTAL_SAMPLERS, samplers);
}

} // namespace dx

} // namespace zge

#endif
