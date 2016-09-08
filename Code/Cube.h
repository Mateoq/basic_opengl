#pragma once

// STL
#include <iostream>
#include <string>

// GLAD
#include <glad/glad.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#define VERTICES_ATTRIB_INDEX 0
#define NORMAL_ATTRIB_INDEX 1
#define TEXTURE_ATTRIB_INDEX 2

struct CubeProperties {
  glm::vec3 color;
 
  glm::vec3 position;
};

class Cube {
 public:
  Cube(std::string shaderName,
       CubeProperties properties);
  ~Cube();

  void draw();

 private:
  CubeProperties* mProperties;

  void init();
};

