Texture2D depthMap : t0;
SamplerState pointClampSampler : s0;

cbuffer ShowDepth
{
  bool showDepth;
};

struct PixelInput
{
  float4 pos : POS;
  float2 tex : TEXCOORD0;
};

float4 main(PixelInput input) : SV_Target
{
  if (showDepth)
  {
    color = 1.0f - depthMap.Sample(pointClampSampler, input.tex.xy);
  }
  else
  {
    color = input.pos;
  }
}
