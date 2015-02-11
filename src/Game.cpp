#include <iostream>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "OpenGLShader.h"
// #include "SimpleVS.h"
// #include "UniformScaleVS.h"
// #include "RedPS.h"
#include "InterpolatedVS.h"
#include "InterpolatedPS.h"

namespace zge
{

using namespace glm;
using namespace std;

Game::Game()
{
  // constructor
}

Game::~Game() noexcept
{
  // destructor
}

Game::Game(const Game& other)
{
  // copy constructor
}

Game& Game::operator=(const Game& rhs)
{
  // assignement operator
  if (this == &rhs)
  {
    return *this;
  }

  return *this;
}

Game::Game(Game&& other) noexcept
{
  // move constructor (C++11)
}

Game& Game::operator=(Game&& other) noexcept
{
  // move assignment operator (C++11)
  return *(new Game(other));
}

const int Game::run()
{
  glfwSetErrorCallback(Game::error_callback);

  if (!glfwInit())
  {
    cerr << "GLFW failed to initialise." << endl;
  }

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Test GLFW", NULL, NULL);

  if (!window)
  {
    cerr << "GLFW failed to initialise window." << endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  glewExperimental = true;
  if (glewInit() != GLEW_OK)
  {
    cerr << "GLEW failed to initialise." << endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwSwapInterval(1);
  glfwSetKeyCallback(window, Game::key_callback);

  // glEnable(GL_MULTISAMPLE);
  // glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_GREATER);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glEnable(GL_CULL_FACE);
  // glFrontFace(GL_CW);
  // glCullFace(GL_BACK);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // uint vertexArrayID;
  // glGenVertexArrays(1, &vertexArrayID);
  // glBindVertexArray(vertexArrayID);
  // glBindVertexArray(0);

  vector<vec3> vertices;
  vertices.push_back(vec3(-1.0f, -1.0f, 0.0f));
  vertices.push_back(vec3(-1.0f, 1.0f, 0.0f));
  vertices.push_back(vec3(1.0f, 1.0f, 0.0f));
  vertices.push_back(vec3(1.0f, -1.0f, 0.0f));

  // uint vertexBuffer = createVertexBuffer(vertices);

  // uint indexBuffer = createIndexBuffer(vector<uint>{ 0, 1, 2, 2, 3, 0 });

  uint objectBuffer =
      createVertexArray(vertices, vector<uint>{ 0, 1, 2, 2, 3, 0 });

  const uint programID = compileShaders(InterpolatedVS, InterpolatedPS);
  glUseProgram(programID);
  m_worldMatrixLocation = glGetUniformLocation(programID, "worldMatrix");
  assert(m_worldMatrixLocation != 0xFFFFFFFF);

  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);
    static float scale = 0.2f;

    // scale += 0.01f;
    // mat4 worldMatrix =
    //     glm::scale(mat4(), vec3(sinf(scale), cosf(scale), atanf(scale)));
    mat4 worldMatrix = glm::scale(mat4(), vec3(scale, scale, scale));
    glUniformMatrix4fv(m_worldMatrixLocation, 1, GL_TRUE, &worldMatrix[0][0]);

    // glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBindVertexArray(objectBuffer);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glDeleteBuffers(1, &vertexBuffer);
  // glDeleteBuffers(1, &indexBuffer);
  glDeleteVertexArrays(1, &objectBuffer);
  glDeleteProgram(programID);

  glfwDestroyWindow(window);
  glfwTerminate();

  return EXIT_SUCCESS;
}

void Game::error_callback(int error, const char* description)
{
  cerr << description << endl;
}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action,
                        int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

const unsigned int Game::createVertexArray(
    const std::vector<glm::vec3> vertices,
    const std::vector<unsigned int> indices) const
{
  uint IDs[3];
  // vertexBuffer
  glGenBuffers(2, IDs);
  glBindBuffer(GL_ARRAY_BUFFER, IDs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(), vertices.data(),
               GL_STATIC_DRAW);
  // indexBuffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IDs[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(),
               indices.data(), GL_STATIC_DRAW);

  glGenVertexArrays(1, &IDs[2]);
  glBindVertexArray(IDs[2]);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, IDs[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IDs[1]);

  glBindVertexArray(0);
  glDeleteBuffers(2, IDs);

  return IDs[2];
}

const uint Game::createInstanceBuffer(const vector<mat4> instances) const
{

  uint result;
  glGenBuffers(1, &result);
  // TODO
  // glBindBuffer(GL_ARRAY_BUFFER, result);
  // glBufferData(GL_ARRAY_BUFFER,
  // sizeof(instances.data()),&instances[0], GL_STATIC_DRAW);

  return result;
}

} // namespace zge
