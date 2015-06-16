#ifndef LIGHT_H
#define LIGHT_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "GameObject.h"

namespace zge
{
enum LightType
{
  DIRECTIONAL_LIGHT,
  POINT_LIGHT,
  SPOT_LIGHT
};

class Light : public GameObject
{
private:
  glm::vec3 lightDirection; // only used if directional light
  glm::vec4 ambientColor;
  glm::vec4 diffuseColor;
  glm::vec4 specularColor;
  float specularPower;
  LightType lightType;
  bool turnedOn;

public:
  // constructor
  Light();
  // destructor
  virtual ~Light() NOEXCEPT;
  // copy constructor
  Light(const Light&);
  // assignment operator
  Light& operator=(const Light&);
  // move constructor (C++11)
  Light(Light&&) NOEXCEPT;
  // move assignment operator (C++11)
  Light& operator=(Light&&) NOEXCEPT;

  void setLightDirection(const glm::vec3);
  glm::vec3 getLightDirection() const;
  void setAmbientColor(const glm::vec4);
  glm::vec4 getAmbientColor() const;
  void setDiffuseColor(const glm::vec4);
  glm::vec4 getDiffuseColor() const;
  void setSpecularColor(const glm::vec4);
  glm::vec4 getSpecularColor() const;
  void setSpecularPower(const float);
  float getSpecularPower() const;
  void turnOn();
  void turnOff();
  const bool isTurnOn();
  void setLightType(const LightType);
  const LightType getLightType() const;

  void update(double deltaTime);
  void draw();

  void addInstance(IShaderProgram*);
};

} // namespace zge

#endif
