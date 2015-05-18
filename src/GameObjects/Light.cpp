#include "Light.h"

namespace zge
{
Light::Light()
    : GameObject(),
      lightDirection({ 0.0f, 0.0f, -1.0f }),
      ambientColor({ 0.0f, 0.0f, 0.0f, 1.0f }),
      diffuseColor({ 0.0f, 0.0f, 0.0f, 1.0f }),
      specularColor({ 0.0f, 0.0f, 0.0f, 1.0f }),
      specularPower(0.0f),
      lightType(DIRECTIONAL_LIGHT),
      turnedOn(true)
{
  // constructor
}

Light::~Light() NOEXCEPT
{
  // destructor
}

Light::Light(const Light& other)
{
  // copy constructor
  this->setPosition(other.getPosition());
  this->ambientColor = other.ambientColor;
  this->lightDirection = other.lightDirection;
  this->specularColor = other.specularColor;
  this->specularPower = other.specularPower;
  this->lightType = other.lightType;
  this->turnedOn = other.turnedOn;
}

Light& Light::operator=(const Light& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

Light::Light(Light&& other) NOEXCEPT
{
  // move constructor (C++11)
}

Light& Light::operator=(Light&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new Light(other));
}

void Light::setLightDirection(const glm::vec3 lightDirection)
{
  this->lightDirection = lightDirection;
}

glm::vec3 Light::getLightDirection() const { return this->lightDirection; }

void Light::setAmbientColor(const glm::vec4 ambientColor)
{
  this->ambientColor = ambientColor;
}

glm::vec4 Light::getAmbientColor() const { return this->ambientColor; }

void Light::setDiffuseColor(const glm::vec4 diffuseColor)
{
  this->diffuseColor = diffuseColor;
}

glm::vec4 Light::getDiffuseColor() const { return this->diffuseColor; }

void Light::setSpecularColor(const glm::vec4 specularColor)
{
  this->specularColor = specularColor;
}

glm::vec4 Light::getSpecularColor() const { return this->specularColor; }

void Light::setSpecularPower(const float specularPower)
{
  this->specularPower = specularPower;
}

float Light::getSpecularPower() const { return this->specularPower; }

void Light::turnOn() { this->turnedOn = true; }

void Light::turnOff() { this->turnedOn = false; }

const bool Light::isTurnOn() { return this->turnedOn; }

void Light::setLightType(const LightType lightType)
{
  this->lightType = lightType;
}

const LightType Light::getLightType() const { return this->lightType; }

void Light::update() {}

void Light::draw() {}

void Light::addInstance(IShaderProgram*) {}

} // namespace zge
