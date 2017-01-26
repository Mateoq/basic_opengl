#pragma once

#include <string>
#include <vector>

// GLAD
#include <glad/glad.h>

// GLM
#include <glm/glm.hpp>

// ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"

#define VERTEX_ATTRIB_INDEX 0
#define NORMAL_ATTRIB_INDEX 1
#define TEXTURE_ATTRIB_INDEX 2

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

struct Texture {
  GLuint id;
  std::string type;
  aiString path;
};

class Mesh {
 public:
  Mesh(std::vector<Vertex *> vertices, std::vector<GLuint *> indices, std::vector<Texture *> textures);

  void draw(Shader* shader);

  // Getters
  inline std::vector<Vertex *> getVertices() { return this->mVertices; }
  inline std::vector<GLuint *> getIndices() { return this->mIndices; }
  inline std::vector<Texture *> getTextures() { return this->mTextures; }

 private:
  // Render Data
  GLuint mVAO, mVBO, mEBO;
  // Mesh Data
  std::vector<Vertex *> mVertices;
  std::vector<GLuint *> mIndices;
  std::vector<Texture *> mTextures;

  void setupMesh();
};

