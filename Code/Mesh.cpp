Mesh::Mesh(std::vector<Vertex> vertices,
	   std::vector<GLuint> indices,
	   std::vector<Texture> textures) : mVertices(vertices),
					    mIndices(indices),
					    mTextures(textures) {
  this->setupMesh();
}

void Mesh::setupMesh() {
  glGetVertexArrays(1, &this->mVAO);
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
			(GLvoid *)offsetof(Vertex, texCoords))
}

