#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in mat4 instanceMatrix;

uniform mat4 worldMatrix;

out vec4 pos;

void main()
{
  pos = vec4(clamp(position, 0.0, 1.0), 1.0);
  gl_Position = worldMatrix * instanceMatrix * vec4(position, 1.0);
}
