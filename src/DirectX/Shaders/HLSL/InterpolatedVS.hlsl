cbuffer MatrixBuffer : register(b0)
{
  matrix worldMatrix;
};

struct VertexInput
{
  float3 position : POSITION;
  matrix instanceMatrix : INSTANCE_MATRIX;
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
  output.position = float4(input.position, 1.0f);
  output.position = mul(output.position, worldMatrix);
  output.position = mul(output.position, input.instanceMatrix);
  output.pos = float4(clamp(input.position, 0.0f, 1.0f), 1.0f);
  output.tex = output.position.xy;

  return output;
}
