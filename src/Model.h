#pragma once

// STD
#include <vector>
#include <string>
#include <iostream>

// GLAD
#include <glad/glad.h>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model {
 public:
  // Constructor, expects a filepath to a 3D model
  Model(std::string path);

  // Draws the model, and thus all its meshes
  void draw(Shader* shader);

 private:
  // Model data
  std::vector<Mesh *> mMeshes;
  std::string mDirectory;
  std::vector<Texture *> mTexturesLoaded;

  // Loads a model with supported ASSIMP extensions from file and stores
  // the resulting meshes in the meshes vector.
  void loadModel(std::string path);

  // Processes a node in a recursive fashion. Processes each individual mesh
  // located at the node and repeats this process on its children nodes (if any)
  void processNode(aiNode* node, const aiScene* scene);
  Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

  // Checks all material textures of a given type and loads the textures if they're not loaded yet
  // The required info is returned as a Texture struct
  std::vector<Texture *> loadMaterialTextures(aiMaterial* material,
					    aiTextureType type,
					    std::string typeName);
};

