#include "Cube.h"

namespace Shapes {
  Cube::Cube() {
    glGenVertexArrays(1, &this->m_VAO);
    glGenBuffers(1, &this->m_VBO);
  }

  void Cube::setUp(Shader& shader) {
    this->m_Shader = shader;
    
    glBindVertexArray(this->m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->m_Vertices), &this->m_Vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(Attribs::VERTICES);
    glVertexAttribPointer(Attribs::VERTICES, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(Attribs::TEX_COORDS);
    glVertexAttribPointer(Attribs::TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);
  }

  void Cube::setTexture(TextureType type, const GLuint texture) {
    switch(type) {
    case TextureType::DIFFUSE:
      this->m_DiffTexture = texture;
      break;
    case TextureType::SPECULAR:
      this->m_SpecularTexture = texture;
      break;
    default:
      std::cout << "Wrong texture type" << std::endl;
      break;
    }
  }
}

