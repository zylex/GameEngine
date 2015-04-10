#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"

namespace zge
{

class Camera : public GameObject
{
public:
  // constructor
  Camera();
  // destructor
  virtual ~Camera() noexcept;
  // copy constructor
  Camera(const Camera&);
  // assignment operator
  Camera& operator=(const Camera&);
  // move constructor (C++11)
  Camera(Camera&&) noexcept;
  // move assignment operator (C++11)
  Camera& operator=(Camera&&) noexcept;

  void update();
  void draw();
};

} // namespace zge

#endif