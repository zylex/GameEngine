// #include <iostream>
// #include <glm/gtx/string_cast.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include "IGame.h"
#include "IResourceManager.h"

#include "Camera.h"

namespace zge
{

Camera::Camera()
    : cameraFront(0.0f, 0.0f, -1.0f),
      cameraUp(0.0f, 1.0f, 0.0f),
      cameraSpeed(5.0f),
      sensitivity(0.05f)
{

  // constructor
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
  glm::vec3 front;
  front.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
  front.y = sin(glm::radians(rotation.x));
  front.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
  this->cameraFront = glm::normalize(front);
}

void Camera::turnRight()
{
  glm::vec3 rotation = this->getRotation();
  rotation.y += this->sensitivity;
  glm::vec3 front;
  front.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
  front.y = sin(glm::radians(rotation.x));
  front.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
  this->cameraFront = glm::normalize(front);
}

void Camera::turnUp()
{
  glm::vec3 rotation = this->getRotation();
  rotation.x += this->sensitivity;
  glm::vec3 front;
  front.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
  front.y = sin(glm::radians(rotation.x));
  front.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
  this->cameraFront = glm::normalize(front);
}

void Camera::turnDown()
{
  glm::vec3 rotation = this->getRotation();
  rotation.x -= this->sensitivity;
  glm::vec3 front;
  front.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
  front.y = sin(glm::radians(rotation.x));
  front.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
  this->cameraFront = glm::normalize(front);
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
  glm::vec3 cameraPos = this->getPosition();

  glm::mat4 indentityMatrix =
      *IResourceManager::getInstance()->getIdentityMatrix();
  glm::mat4 viewMatrix =
      glm::lookAt(cameraPos, cameraPos + this->cameraFront, this->cameraUp);
  glm::mat4 proj = glm::perspective(
      45.0f, (float)SCREEN_WIDTH() / (float)SCREEN_HEIGHT(), 0.1f, 1000.0f);
  return viewMatrix;
}

void Camera::addInstance(IShaderProgram* shaderProgram) {}

} // namespace zge
