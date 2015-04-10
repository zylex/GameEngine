Texture2D depthMap : register(t2);
SamplerState pointClampSampler : register(s0);

cbuffer ShowDepth : register(b1)
{
  bool showDepth;
};

struct PixelInput
{
  float4 position : SV_POSITION;
  float4 pos : POS;
  float2 tex : TEXCOORD0;
};

float4 main(PixelInput input) : SV_Target
{
  float4 color;
  if (showDepth)
  {
    color = 1.0f - depthMap.Sample(pointClampSampler, input.tex.xy);
  }
  else
  {
    color = input.pos;
  }
  return color;
}
