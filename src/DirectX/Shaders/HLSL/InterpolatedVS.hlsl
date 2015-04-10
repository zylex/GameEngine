cbuffer MatrixBuffer : register(b0)
{
  matrix worldMatrix;
};

struct VertexInput
{
  float3 position : POSITION;
  matrix instanceMatrix : WORLD_MATRIX;
};

struct PixelInput
{
  float4 position : SV_POSITION;
  float4 pos : POS;
  float2 tex : TEXCOORD0;
};

PixelInput main(VertexInput input)
{
  PixelInput output;
  output.pos = float4(clamp(input.position, 0.0, 1.0), 1.0);
  output.position = mul(mul(float4(input.position, 1.0), worldMatrix), input.instanceMatrix);
  output.tex = output.position.xy;
  return output;
}
