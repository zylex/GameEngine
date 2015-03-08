#ifndef OPEN_GL_GAME_H
#define OPEN_GL_GAME_H

#include <vector>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace zge
{
namespace gl
{

class OpenGLGame
{
public:
  // constructor
  OpenGLGame(std::string);
  // destructor
  virtual ~OpenGLGame() noexcept;
  // copy constructor
  OpenGLGame(const OpenGLGame& other);
  // assignment operator
  OpenGLGame& operator=(const OpenGLGame& other);
  // move constructor (C++11)
  OpenGLGame(OpenGLGame&& other) noexcept;
  // move assignment operator (C++11)
  OpenGLGame& operator=(OpenGLGame&& other) noexcept;

  const int run();

  static void error_callback(int, const char*);
  static void key_callback(GLFWwindow*, int, int, int, int);

private:
  std::string m_applicationName;
  static bool showDepth;
  unsigned int m_worldMatrixLocation;
  unsigned int m_worldMatrixShadowLocation;
  unsigned int m_showDepthPosition;
  unsigned int m_depthMapPosition;
  unsigned int m_depthMapTexture;
  unsigned int m_depthFramebuffer;

  const unsigned int createVertexArray(const std::vector<glm::vec3>,
                                       const std::vector<unsigned int>,
                                       const unsigned int) const;
  const unsigned int createInstanceBuffer(const std::vector<glm::mat4>) const;

  const unsigned int createDepthBuffer(const unsigned int) const;
};

} // namespace gl
} // namespace zge

#endif