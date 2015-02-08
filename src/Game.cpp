#include "Game.h"

namespace zge {

Game::Game() {
  // constructor
}

Game::~Game() noexcept {
  // destructor
}

Game::Game(const Game& other) {
  // copy constructor
}

Game& Game::operator=(const Game& rhs) {
  // assignement operator
  if (this == &rhs) {
    return *this;
  }

  return *this;
}

Game::Game(Game&& other) noexcept {
  // move constructor (C++11)
}

Game& Game::operator=(Game&& other) noexcept {
  // move assignment operator (C++11)
  return *(new Game(other));
}

int Game::run() {
  glfwSetErrorCallback(Game::error_callback);

  if (!glfwInit()) {
    std::cerr << "GLFW failed to initialise." << std::endl;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Test GLFW", NULL, NULL);

  if (!window) {
    std::cerr << "GLFW failed to initialise window." << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    std::cerr << "GLEW failed to initialise." << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwSwapInterval(1);
  glfwSetKeyCallback(window, Game::key_callback);

  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}

void Game::error_callback(int error, const char* description) {
  std::cerr << description << std::endl;
}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action,
                        int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

} // namespace zge
