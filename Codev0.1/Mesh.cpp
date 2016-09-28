#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex *> vertices,
	   std::vector<GLuint *> indices,
	   std::vector<Texture *> textures) : mVertices(vertices),
					    mIndices(indices),
					    mTextures(textures) {
  this->setupMesh();
}

void Mesh::draw(Shader* shader) {
  GLuint diffuseNumber = 1;
  GLuint specularNumber = 1;

  for(GLuint i = 0; i < this->mTextures.size(); i++) {
    // Activate proper texture unit before binding
    glActiveTexture(GL_TEXTURE0 + i);
    // Retrive texture number (the N in diffuse_textureN)
    std::stringstream stringStream;
    std::string number;
    std::string name = this->mTextures[i]->type;

    if(name == "texture_diffuse") {
      stringStream << diffuseNumber++; // Transfer GLuint to stream
    } else if(name == "texture_specular") {
      stringStream << specularNumber++; // Transfer GLuint to stream
    }

    number = stringStream.str();
    // Now set the sampler to the correct texture unit
    glUniform1i(glGetUniformLocation(shader->program, (name + number).c_str()), i);
    // And finally bind the texture
    glBindTexture(GL_TEXTURE_2D, this->mTextures[i]->id);
  }

  // Also set each mesh's shininess property to a default value
  // (if you want you could extend this to another mesh property and possibly change this value)
  glUniform1i(glGetUniformLocation(shader->program, "material.shininess"), 16.0f);

  // Draw mesh
  glBindVertexArray(this->mVAO);
  glDrawElements(GL_TRIANGLES, this->mIndices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // Always good practice to set everything back to defaults once configured
  for(GLuint i = 0; i < this->mTextures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void Mesh::setupMesh() {
  glGenVertexArrays(1, &this->mVAO);
  glGenBuffers(1, &this->mVBO);
  glGenBuffers(1, &this->mEBO);

  glBindVertexArray(this->mVAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->mVBO);

  glBufferData(GL_ARRAY_BUFFER, this->mVertices.size() * sizeof(Vertex),
	       &this->mVertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mIndices.size() * sizeof(GLuint),
	       &this->mIndices[0], GL_STATIC_DRAW);

  // Vertex Positions
  glEnableVertexAttribArray(VERTEX_ATTRIB_INDEX);
  glVertexAttribPointer(VERTEX_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid *)0);

  // Vertex Normals
  glEnableVertexAttribArray(NORMAL_ATTRIB_INDEX);
  glVertexAttribPointer(NORMAL_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid *)offsetof(Vertex, normal));

  // Vertex Texture Coords
  glEnableVertexAttribArray(TEXTURE_ATTRIB_INDEX);
  glVertexAttribPointer(TEXTURE_ATTRIB_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid *)offsetof(Vertex, texCoords));

  glBindVertexArray(0);
}

