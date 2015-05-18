// #include <iostream>
// #include <glm/gtx/string_cast.hpp>

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "IGame.h"
#include "IResourceManager.h"

#include "Camera.h"

namespace zge
{

const glm::vec4 Camera::UP = { 0.0f, 1.0f, 0.0f, 1.0f };
const glm::vec4 Camera::FORWARD = { 0.0f, 0.0f, -1.0f, 1.0f };

Camera::Camera()
    : cameraFront(Camera::FORWARD),
      cameraUp(Camera::UP),
      cameraSpeed(1.0f),
      // sensitivity(0.05f)
      sensitivity(5.0f)
{
  // constructor
  // glm::vec3 rotation = this->getRotation();
  // rotation.y = 90.0f;
  // this->cameraFront.x =
  //     cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
  // this->cameraFront.y = sin(glm::radians(rotation.x));
  // this->cameraFront.z =
  //     sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
  // this->cameraFront = glm::normalize(this->cameraFront);
  // this->setRotation({ 0.0f, 0.0f, 0.0f });
}

Camera::~Camera() NOEXCEPT
{

  // destructor
}

Camera::Camera(const Camera& other)
{
  // copy constructor
}

Camera& Camera::operator=(const Camera& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

Camera::Camera(Camera&& other) NOEXCEPT
{

  // move constructor (C++11)
}

Camera& Camera::operator=(Camera&& other) NOEXCEPT
{
  // move assignment operator (C++11)
  return *(new Camera(other));
}

void Camera::update() {}

void Camera::draw() {}

void Camera::setCameraSpeed(float cameraSpeed)
{
  this->cameraSpeed = cameraSpeed;
}

const float Camera::getCameraSpeed() const { return this->cameraSpeed; }

void Camera::strafeLeft()
{
  glm::vec3 cameraPos = this->getPosition();
  cameraPos -= glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) *
               this->cameraSpeed;
  this->setPosition(cameraPos);
}

void Camera::strafeRight()
{
  glm::vec3 cameraPos = this->getPosition();
  cameraPos += glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) *
               this->cameraSpeed;
  this->setPosition(cameraPos);
}

void Camera::strafeUp()
{
  glm::vec3 cameraPos = this->getPosition();
  cameraPos += this->cameraUp * this->cameraSpeed;
  this->setPosition(cameraPos);
}

void Camera::strafeDown()
{
  glm::vec3 cameraPos = this->getPosition();
  cameraPos -= this->cameraUp * this->cameraSpeed;
  this->setPosition(cameraPos);
}

void Camera::turnLeft()
{
  glm::vec3 rotation = this->getRotation();
  rotation.y -= this->sensitivity;
  this->setRotation(rotation);
}

void Camera::turnRight()
{
  glm::vec3 rotation = this->getRotation();
  rotation.y += this->sensitivity;
  this->setRotation(rotation);
}

void Camera::turnUp()
{
  glm::vec3 rotation = this->getRotation();
  rotation.x -= this->sensitivity;
  this->setRotation(rotation);
}

void Camera::turnDown()
{
  glm::vec3 rotation = this->getRotation();
  rotation.x += this->sensitivity;
  this->setRotation(rotation);
}

void Camera::moveForward()
{
  glm::vec3 cameraPos = this->getPosition();
  cameraPos += this->cameraSpeed * this->cameraFront;
  this->setPosition(cameraPos);
}

void Camera::moveBackward()
{
  glm::vec3 cameraPos = this->getPosition();
  cameraPos -= this->cameraSpeed * this->cameraFront;
  this->setPosition(cameraPos);
}

glm::mat4 Camera::getViewMatrix()
{
  glm::vec3 rotation = this->getRotation();
  glm::mat4 rotationMatrix =
      glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x),
                         glm::radians(rotation.z));
  this->cameraFront =
      glm::normalize(glm::vec3(Camera::FORWARD * rotationMatrix));
  this->cameraUp = glm::normalize(glm::vec3(Camera::UP * rotationMatrix));
  glm::vec3 cameraPos = this->getPosition();

  return glm::lookAt(cameraPos, cameraPos + this->cameraFront, this->cameraUp);
}

void Camera::addInstance(IShaderProgram* shaderProgram) {}

} // namespace zge
