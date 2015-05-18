#version 430

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTextureCoordinates;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in mat4 instanceMatrix;

layout(binding = 0) uniform MatrixBuffer
{
  mat4 viewMatrix;
  mat4 projectionMatrix;
};

out vec4 position;
out vec2 texCoords;
out vec3 normal;

void main()
{
position = instanceMatrix * vec4(vertexPosition, 1.0f);

normal = normalize(transpose(inverse(mat3(instanceMatrix))) * vertexNormal);
texCoords = vec2(1.0 - vertexTextureCoordinates.x, vertexTextureCoordinates.y);

gl_Position = projectionMatrix * viewMatrix * position;
}
