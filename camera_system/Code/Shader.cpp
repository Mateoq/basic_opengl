#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
  // Retrive the vertex/fragment source code from filepath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vertexShaderFile;
  std::ifstream fragmentShaderFile;

  // Ensures ifstream objects can throw exceptions
  vertexShaderFile.exceptions(std::ifstream::badbit);
  fragmentShaderFile.exceptions(std::ifstream::badbit);

  try {
    // Open files
    vertexShaderFile.open(vertexPath);
    fragmentShaderFile.open(fragmentPath);

    std::stringstream vertexShaderStream, fragmentShaderStream;

    // Read file's buffer contents into streams
    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();

    // Close file handlers
    vertexShaderFile.close();
    fragmentShaderFile.close();

    // Convert stream into GLchar array
    vertexCode = vertexShaderStream.str();
    fragmentCode = fragmentShaderStream.str();
  } catch(std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
  }

  const GLchar* vertexShaderCode = vertexCode.c_str();
  const GLchar* fragmentShaderCode = fragmentCode.c_str();

  // Compile Shaders
  GLuint vertex, fragment;
  GLint success;
  GLchar infoLog[512];

  // Vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexShaderCode, nullptr);
  glCompileShader(vertex);

  // Print compile errors if any
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentShaderCode, nullptr);
  glCompileShader(fragment);

  // Print compile errors if any
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Shader Program
  this->program = glCreateProgram();
  glAttachShader(this->program, vertex);
  glAttachShader(this->program, fragment);

  glLinkProgram(this->program);

  // Print linking errors if any
  glGetProgramiv(this->program, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(this->program, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  // Delete the shaders as they're linked into the program and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() {
  glUseProgram(this->program);
}

void Shader::unuse() {
  glUseProgram(0);
}

