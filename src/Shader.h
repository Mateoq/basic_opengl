#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

class Shader {
 public:
  // Default constructor
  Shader() {};
  // Constructor reads and builds the shader
  Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
  // Use the program
  void use();
  //Dispose the program
  void unuse();

  GLuint getProgram() { return this->m_Program; } 
  
 private:
  // The program ID
  GLuint m_Program;
};

