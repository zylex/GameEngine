#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>

namespace zge {

class Game {
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

  const int run() const;

  static void error_callback(int, const char*);
  static void key_callback(GLFWwindow*, int, int, int, int);
};

} // namespace zge

#endif