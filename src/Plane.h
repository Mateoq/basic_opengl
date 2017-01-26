#pragma once

// STD
#include <iostream>
#include <vector>

// GLAD
#include <glad/glad.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "World.h"
#include "Shader.h"
#include "Texture.h"
#include "Constants.h"

namespace Graphics {
  class Plane {
  public:
    Plane();

    void setUp(Shader& shader);

    void setTexture(Texture newTexture);

    // Matrix Modifiers
    void scale(glm::vec3 scaleVector);
    void rotate(glm::vec3 rotateDirection, float angle);
    void translate(glm::vec3 translateVector);

    // Core functionality
    void update(World& world);
    void render();

  private:
    Shader m_Shader;
    glm::mat4 m_Model;
    glm::mat4 m_View;
    glm::mat4 m_Projection;
    GLuint m_VAO, m_VBO;
    std::vector<Texture> m_Textures;
    const float m_Vertices[] = {
      // Positions          // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
      5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
      -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
      -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

      5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
      -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
      5.0f,  -0.5f, -5.0f,  2.0f, 2.0f
    };
  };
}

