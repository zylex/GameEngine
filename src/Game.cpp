#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Game.h"
#include "OpenGLShader.h"
#include "SimpleVS.h"
#include "RedPS.h"

namespace zge {

using namespace glm;

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

const int Game::run() const {
  glfwSetErrorCallback(Game::error_callback);

  if (!glfwInit()) {
    std::cerr << "GLFW failed to initialise." << std::endl;
  }

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Test GLFW", NULL, NULL);

  if (!window) {
    std::cerr << "GLFW failed to initialise window." << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::cerr << "GLEW failed to initialise." << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwSwapInterval(1);
  glfwSetKeyCallback(window, Game::key_callback);

  // glEnable(GL_MULTISAMPLE);
  // glEnable(GL_DEPTH_TEST);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  uint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  uint programID = compileShaders(SimpleVS, RedPS);

  vec3 vertices[3];
  vertices[0] = vec3(-1.0f, -1.0f, 0.0f);
  vertices[1] = vec3(1.0f, -1.0f, 0.0f);
  vertices[2] = vec3(0.0f, 1.0f, 0.0f);

  uint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  while (!glfwWindowShouldClose(window)) {

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(programID);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &VertexArrayID);
  glDeleteProgram(programID);

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
