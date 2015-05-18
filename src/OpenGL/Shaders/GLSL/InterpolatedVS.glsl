#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 instanceMatrix;

layout(binding = 0) uniform MatrixBuffer
{
  mat4 worldMatrix;
  mat4 projectionMatrix;
};

// layout(location = 0) uniform mat4 worldMatrix;

out vec4 pos;
// out vec2 tex;

void main()
{
  pos = vec4(clamp(position, 0.0, 1.0), 1.0);
  mat4 transformMatrix = projectionMatrix * worldMatrix * instanceMatrix;
  gl_Position = vec4(position, 1.0) * transformMatrix;
  // tex = gl_Position.xy;
}
