#include "Cube.h"

namespace Graphics {
  Cube::Cube() {
    this->m_Model = glm::mat4();
    this->m_View = glm::mat4();
    this->m_Projection = glm::mat4();
    
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

  void Cube::setTexture(Texture& newTexture) {
    this->m_Textures.push_back(newTexture);
  }

  // Matrix Modifiers
  void Cube::scale(glm::vec3 scaleVector) {
    this->m_Model = glm::sale(this->m_Model, scaleVector);
  }
  
  void Cube::rotate(glm::vec3 rotateDirection, float angle) {
    this->m_Model = glm::rotate(this->m_Model, angle, rotateDirection);
  }
  
  void Cube::translate(glm::vec3 translateVector) {
    this->m_Model = glm::translate(this->m_Model, translateVector);
  }

  // Core functionality.
  // Update the object matrices
  void Cube::update(World& world) {
    this->m_View = world.camera.getViewMatrix();
    this->m_Projection = glm::perspective(world.camera.zoom,
					  (float)world.screenWidth / (float)world.screenHeight,
					  0.1f, 100.f);
  }
  
  void Cube::render() {
    // Activate the shader program
    this->m_Shader.use();

    // Set uniform param to the view matrix
    glUniformMatrix4fv(glGetUniformLocation(this->m_Shader.getProgram(), "view"),
		       1, GL_FALSE, glm::value_ptt(this->m_View));

    // Set uniform param to the projection matrix
    glUniformMatrix4fv(glGetUniformLocation(this->m_Shader.getProgram(), "projection"),
		       1, GL_FALSE, glm::value_ptt(this->m_Projection));

    // Set uniform param to the model matrix
    glUniformMatrix4fv(glGetUniformLocation(this->m_Shader.getProgram(), "model"),
		       1, GL_FALSE, glm::value_ptt(this->m_Model));

    // Bind vertex array
    glBindVertexArray(this->m_VAO);

    // Bind textures
    for (auto& texture : this->m_Textures) {
      glBindTexture(GL_TEXTURE_2D, texture.id);
    }

    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);

    this->m_Shader.unuse();
  }
}

