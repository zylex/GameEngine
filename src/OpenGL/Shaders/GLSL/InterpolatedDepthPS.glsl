#version 430

in vec4 pos;
in vec2 tex;
// worldMatrix is location 0
layout(binding = 1) uniform ShowDepthBuffer
{
 bool showDepth;
};

layout(binding = 0) uniform sampler2D depthMap;

out vec4 color;

void main()
{
  if (showDepth)
  {
    color = 1.0f - texture(depthMap, tex.xy);
  }
  else
  {
    color = pos;
  }
}
