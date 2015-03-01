#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in mat4 instanceMatrix;

uniform mat4 worldMatrix;

void main(){
  gl_Position = vec4(position, 1.0) * worldMatrix * instanceMatrix;
}

