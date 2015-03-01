#version 330

in vec4 pos;
in vec2 tex;
uniform bool showDepth;
uniform sampler2D depthMap;

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
