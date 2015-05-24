#version 430

in vec4 position;
in vec2 texCoords;
in vec3 normal;

layout(binding = 1) uniform LightBuffer
{
  vec4 ambientColor;
  vec4 diffuseColor;
  vec4 specularColor;
  vec3 vector; // position if point light, direction if directional
  float specularPower;
  vec3 viewPosition;
  int lightType; // 0 for directional, 1 for point, 2 for other

} Light;

layout(binding = 0) uniform sampler2D tex;

out vec4 outputColor;

void main()
{
  outputColor = texture(tex, texCoords);
  vec3 lightDirection;
  if (Light.lightType < 1)
  {
    lightDirection = normalize(-Light.vector);
  }
  else if (Light.lightType < 2)
  {
    lightDirection = normalize(Light.vector - position.xyz);
  }

  vec4 L = normalize(vec4(lightDirection, 0.0f));
  vec4 V = normalize(-vec4(position.xyz - Light.viewPosition, 0.0f));
  vec4 H = normalize(L + V);
  float n = 32.0f;

  vec4 diffuse = max(dot(L.xyz, normal), 0.0) * Light.diffuseColor;
  vec4 specular = pow(max(dot(H.xyz, normal), 0.0), n) * Light.specularPower * Light.specularColor;
  outputColor *= diffuse + specular + Light.ambientColor;
  outputColor.a = 0.7f;
  // outputColor = vec4(1.0f,0.0f,0.0f,1.0f);
}
