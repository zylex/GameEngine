#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"

namespace zge
{

class Camera : public GameObject
{
private:
  glm::vec3 cameraFront;
  glm::vec3 cameraUp;
  float cameraSpeed;
  float sensitivity;

  static const glm::vec4 UP;
  static const glm::vec4 FORWARD;

public:
  // constructor
  Camera();
  // destructor
  virtual ~Camera() NOEXCEPT;
  // copy constructor
  Camera(const Camera&);
  // assignment operator
  Camera& operator=(const Camera&);
  // move constructor (C++11)
  Camera(Camera&&) NOEXCEPT;
  // move assignment operator (C++11)
  Camera& operator=(Camera&&) NOEXCEPT;

  void update(double deltaTime);
  void draw();

  void addInstance(IShaderProgram*);

  void setCameraSpeed(float);
  const float getCameraSpeed() const;

  void strafeLeft();
  void strafeRight();
  void strafeUp();
  void strafeDown();

  void turnLeft();
  void turnRight();
  void turnUp();
  void turnDown();

  void moveForward();
  void moveBackward();

  glm::mat4 getViewMatrix();
};

} // namespace zge

#endif
