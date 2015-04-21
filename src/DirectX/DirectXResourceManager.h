#ifndef DIRECTXRESOURCEMANAGER_H
#define DIRECTXRESOURCEMANAGER_H

#include <d3d11.h>
#include <vector>
#include <glm/vec3.hpp>

#include "SamplerType.h"
#include "RasterState.h"

#include "ResourceManager.h"

namespace zge
{

namespace dx
{
// forward declarations
struct Program;
struct Mesh;
struct Output;

class DirectXResourceManager : public ResourceManager
{
private:
  std::vector<ID3D11Buffer*> buffers;
  std::vector<Mesh> meshes;

  std::vector<ID3D11ComputeShader*> computeShaders;
  std::vector<ID3D11VertexShader*> vertexShaders;
  std::vector<ID3D11GeometryShader*> geometryShaders;
  std::vector<ID3D11HullShader*> hullShaders;
  std::vector<ID3D11DomainShader*> domainShaders;
  std::vector<ID3D11PixelShader*> pixelShaders;
  std::vector<ID3D11InputLayout*> inputLayouts;
  std::vector<Program> shaderPrograms;
  std::vector<std::pair<const void*, std::size_t> > vertexShaderByteCode;

  std::vector<std::pair<ID3D11Texture2D*, ID3D11ShaderResourceView*> >
      shaderTextures;
  std::vector<Output> outputTargets;

  std::unordered_map<unsigned, std::vector<unsigned> > uniformBuffers;

  // MAYBE: these should maybe be vectors instead of arrays?
  ID3D11RasterizerState* rasterStates[TOTAL_RASTER_STATES];         // culling on/off
  ID3D11DepthStencilState* depthStencilStates[3]; // default, read, off
  ID3D11BlendState* blendStates[2];               // alpha blending on/off
  ID3D11SamplerState* samplers[TOTAL_SAMPLERS];

  ID3D11Device* device;

  ID3D11Texture2D* backBufferTexture;

public:
  static DirectXResourceManager* getInstance();
  // destructor
  virtual ~DirectXResourceManager() NOEXCEPT;
  // copy constructor
  DirectXResourceManager(const DirectXResourceManager&);
  // assignment operator
  DirectXResourceManager& operator=(const DirectXResourceManager&);
  // move constructor (C++11)
  DirectXResourceManager(DirectXResourceManager&&) NOEXCEPT;
  // move assignment operator (C++11)
  DirectXResourceManager& operator=(DirectXResourceManager&&) NOEXCEPT;

  const bool initialise();

  const unsigned createMesh(const std::vector<glm::vec3>& /*vertices*/,
                            const std::vector<glm::vec3>& /*normals*/,
                            const std::vector<glm::uvec3>& /*indices*/);
  const unsigned createMesh(
      const std::vector<glm::vec3>& /*vertices*/,
      const std::vector<glm::vec3>& /*normals*/,
      const std::vector<glm::uvec3>& /*indices*/,
      const std::vector<glm::vec2>& /*textureCoordinates*/,
      const std::vector<glm::vec3>& /*tangents*/,
      const std::vector<glm::vec3>& /*bitangents*/);

  const unsigned getInstanceBuffer();

  const unsigned compileShaderCode(const void*, const std::size_t,
                                   const unsigned);
  const unsigned createShaderProgram(const unsigned, const unsigned,
                                     const unsigned, const unsigned,
                                     const unsigned, const unsigned);

  const unsigned createRawTexture2D(const unsigned, const unsigned,
                                    const unsigned);
  const unsigned createOutputBuffer(const unsigned,
                                    const std::vector<unsigned>&);

  const unsigned createInstanceBuffer(const unsigned long);

  const unsigned createInputLayout(
      const unsigned, const std::vector<D3D11_INPUT_ELEMENT_DESC>&);
  //                                 |  ProgramId   |
  // const unsigned createInputLayout(const unsigned,
  //                                 |     input attribute list          |
  //                                  const std::vector<ShaderAttribute>&);

  // getters and setters
  void setDevice(ID3D11Device*);
  bool setBackBufferTexture(ID3D11Texture2D*);

  const Program getProgram(const unsigned) const;
  const Output getOutput(const unsigned) const;
  const Mesh getMesh(const unsigned) const;
  ID3D11ComputeShader* getComputeShader(const unsigned) const;
  ID3D11VertexShader* getVertexShader(const unsigned) const;
  ID3D11GeometryShader* getGeometryShader(const unsigned) const;
  ID3D11HullShader* getHullShader(const unsigned) const;
  ID3D11DomainShader* getDomainShader(const unsigned) const;
  ID3D11PixelShader* getPixelShader(const unsigned) const;
  ID3D11Buffer* getBuffer(const unsigned) const;
  ID3D11DepthStencilState* getDepthState(const unsigned) const;
  ID3D11RasterizerState* getRasterState(const unsigned) const;
  ID3D11ShaderResourceView* getTexture(const unsigned) const;
  ID3D11InputLayout* getInputLayout(const unsigned) const;
  ID3D11InputLayout* getBasicInputLayout() const;

  std::vector<unsigned>* getUniformBuffers(const unsigned programId);
  unsigned createUniformBuffer(const unsigned long bufferSize);

  ID3D11RenderTargetView* createBackBufferOutput(ID3D11Texture2D*);

  const glm::mat4 getIdentityMatrix() const;

private:
  // constructor
  DirectXResourceManager();

  void updateBuffer(const unsigned, const unsigned long, void*);

  const unsigned createVertexBuffer(const std::vector<glm::vec3>&);
  const unsigned createIndexBuffer(const std::vector<glm::uvec3>&);

  ID3D11Buffer* createBuffer(const void*, const D3D11_BUFFER_DESC&);

  const D3D11_BUFFER_DESC getVertexBufferDescription(
      const unsigned long&) const;
  const D3D11_BUFFER_DESC getIndexBufferDescription(const unsigned long&) const;
  const D3D11_BUFFER_DESC getInstanceBufferDescription(
      const unsigned long&) const;
  const D3D11_BUFFER_DESC getConstantBufferDescription(
      const unsigned long&) const;

  const D3D11_TEXTURE2D_DESC getDepthTextureDescription(const unsigned,
                                                        const unsigned) const;
  const D3D11_TEXTURE2D_DESC getColorTextureDescription(const unsigned,
                                                        const unsigned) const;

  const D3D11_DEPTH_STENCIL_VIEW_DESC getDepthStencilViewDescription() const;
  const D3D11_RENDER_TARGET_VIEW_DESC getRenderTargetViewDescription() const;

  const D3D11_SHADER_RESOURCE_VIEW_DESC getShaderResourceViewDescription(
      const DXGI_FORMAT) const;

  // const D3D11_INPUT_ELEMENT_DESC* getLayoutDescription() const;
  std::vector<D3D11_INPUT_ELEMENT_DESC> getLayoutDescription() const;
  // const D3D11_INPUT_ELEMENT_DESC* getBasicLayoutDescription() const;
  std::vector<D3D11_INPUT_ELEMENT_DESC> getBasicLayoutDescription() const;

  ID3D11DepthStencilState* createDepthDisabled() const;
  ID3D11DepthStencilState* createDepthEnabled() const;

  ID3D11RasterizerState* createRasterState(D3D11_CULL_MODE, D3D11_FILL_MODE) const;

  ID3D11SamplerState* createSampler(D3D11_FILTER,
                                    D3D11_TEXTURE_ADDRESS_MODE) const;
};

} // namespace dx

} // namespace zge

#endif
