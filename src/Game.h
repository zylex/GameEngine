#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

namespace zge
{

class Game
{
public:
  // constructor
  Game();
  // destructor
  virtual ~Game() noexcept;
  // copy constructor
  Game(const Game& other);
  // assignment operator
  Game& operator=(const Game& other);
  // move constructor (C++11)
  Game(Game&& other) noexcept;
  // move assignment operator (C++11)
  Game& operator=(Game&& other) noexcept;

  const int run();

  static void error_callback(int, const char*);
  static void key_callback(GLFWwindow*, int, int, int, int);

private:
  unsigned int m_worldMatrixLocation;
  unsigned int m_shaderAttributeLocation;
  const unsigned int createVertexArray(const std::vector<glm::vec3>,
                                       const std::vector<unsigned int>) const;
  // TODO: implement
  const unsigned int createInstanceBuffer(const std::vector<glm::mat4>) const;
};

} // namespace zge

#endif