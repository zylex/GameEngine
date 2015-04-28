#include "Camera.h"

namespace zge
{

Camera::Camera()
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

} // namespace zge
