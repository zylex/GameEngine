#ifdef OPEN_GL

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include "OpenGLGame.h"

namespace zge
{

namespace gl
{

unsigned int compileShaders(const char* vertexShaderCode,
                            const char* pixelShaderCode)
{
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  glShaderSource(VertexShaderID, 1, &vertexShaderCode, NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  if (!Result)
  {
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
      std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
      glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL,
                         &VertexShaderErrorMessage[0]);
      std::cerr << &VertexShaderErrorMessage[0] << std::endl;
    }
  }
  // Compile Fragment Shader
  glShaderSource(FragmentShaderID, 1, &pixelShaderCode, NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  if (!Result)
  {
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
      std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
      glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL,
                         &FragmentShaderErrorMessage[0]);
      std::cerr << &FragmentShaderErrorMessage[0] << std::endl;
    }
  }

  // Link the program
  // printf("Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  if (!Result)
  {
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
      std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
      glGetProgramInfoLog(ProgramID, InfoLogLength, NULL,
                          &ProgramErrorMessage[0]);
      std::cerr << &ProgramErrorMessage[0] << std::endl;
    }
  }

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}

unsigned int compileShader(const char* vertexShaderCode)
{
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  glShaderSource(VertexShaderID, 1, &vertexShaderCode, NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  if (!Result)
  {
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
      std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
      glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL,
                         &VertexShaderErrorMessage[0]);
      std::cerr << &VertexShaderErrorMessage[0] << std::endl;
    }
  }

  // Link the program
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  if (!Result)
  {
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0)
    {
      std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
      glGetProgramInfoLog(ProgramID, InfoLogLength, NULL,
                          &ProgramErrorMessage[0]);
      std::cerr << &ProgramErrorMessage[0] << std::endl;
    }
  }

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}

} // namespace gl
} // namespace zge

#endif