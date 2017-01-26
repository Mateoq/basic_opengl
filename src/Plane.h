#pragma once

#include <iostream>

#include <glad/glad.h>

#include "Constants.h"

namespace Shapes {
  class Plane {
  public:
    Plane();

    void setUp();
    void render();

    void setTexture(TextureType type, const GLuint texture);
    
    float[] getVertices() const { return this->m_Vertices; }
  private:
    GLuint m_VAO, m_VBO;
    GLuint m_DiffTexture;
    GLuint m_SpecularTexture;
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

