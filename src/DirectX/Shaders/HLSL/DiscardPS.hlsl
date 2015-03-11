struct PixelInput
{
  float4 position : SV_POSITION;
  float4 pos : POS;
  float2 tex : TEXCOORD0;
};

float4 main(PixelInput input) : SV_Target
{
  discard;
  return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

