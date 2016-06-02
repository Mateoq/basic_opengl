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
}
