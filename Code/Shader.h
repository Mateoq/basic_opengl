#pragma once

#include <glad/glad.h>
#include <SDL/SDL.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Types.h"

class Shader {
 public:
  std::string readFile(const char* file);

  void bindAttributeLocation(int index, const std::string& attribute);

  void useProgram();

  bool init();

  bool loadVertexShader(const std::string& filename);
  bool loadFragmentShader(const std::string& filename);

  bool linkShaders();

  void printShaderLinkingError(int32 shaderId);
  void printShaderCompilationErrorInfo(int32 shaderId);

  void cleanUp();

  // The handler to the shader program
  GLuint shaderProgram;

  // The handler to individual shader
  GLuint vertexShader, fragmentShader;
};
