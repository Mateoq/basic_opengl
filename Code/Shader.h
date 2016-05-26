#pragma once

#include <glad/glad.h>
#include <SDL/SDL.h>

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Types.h"

class Shader {
 public:
  bool init();
  bool loadShader(const std::string& fileName, GLenum shaderType);
  void useProgram();
  void bindAttributeLocation(int index, const std::string& attribute);
  void cleanUp();

 private:
  // Tries to compile the shader. Returns false if something fails
  bool tryCompileShader(GLuint shaderId);
  // Create shader and set the source
  int createShader(const std::string& fileName, GLenum shaderType);
  bool linkShaders();
  std::string readFile(const char* file);
  void printShaderLinkingError();
  void printShaderCompilationErrorInfo(GLuint shaderId);

  // The handler to the shader program
  GLuint shaderProgram;

  // The handler to individual shader
  std::vector<GLuint> shaderIds;
};
