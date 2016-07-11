#include "Model.h"

Model::Model(std::string path) {
  this->loadModel(path);
}

void Model::draw(Shader* shader) {
  for(GLuint i = 0; i < this->mMeshes.size(); i++) {
    this->mMeshes[i]->draw(shader);
  }
}

void Model::loadModel(std::string path) {
  // Read file via ASSIMP
  Assimp::Importer import;
  const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  // Check for errors
  if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }

  // Retrieve the directory path of the filepath
  this->mDirectory = path.substr(0, path.find_last_of('/'));

  // Process ASSIMP's root node recursively
  this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
  // Process each mesh located at the current node
  for(GLuint meshIndex = 0; meshIndex < node->mNumMeshes; meshIndex++) {
    // The node object only contains indices to index the actual objects in the scene.
    // The scene contains all the data, node is just to keep stuff organized (like relations between nodes)
    aiMesh* mesh = scene->mMeshes[node->mMeshes[meshIndex]];
    this->mMeshes.push_back(this->processMesh(mesh, scene));
  }
  // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
  for(GLuint childIndex = 0; childIndex < node->mNumChildren; childIndex++) {
    this->processNode(node->mChildren[childIndex], scene);
  }
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene) {
  // Data to fill
  std::vector<Vertex *> vertices = {};
  std::vector<GLuint *> indices = {};
  std::vector<Texture *> textures = {};

  // Walk throug each of the mesh's vertices
  for(GLuint vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
    Vertex vertex = {};
    // Process vertex positions, normals and texture coordinates
    glm::vec3 vector;

    // Positions
    vector.x = mesh->mVertices[vertexIndex].x;
    vector.y = mesh->mVertices[vertexIndex].y;
    vector.z = mesh->mVertices[vertexIndex].z;
    vertex.position = vector;

    // Normals
    vector.x = mesh->mNormals[vertexIndex].x;
    vector.y = mesh->mNormals[vertexIndex].y;
    vector.z = mesh->mNormals[vertexIndex].z;
    vertex.normal = vector;

    // Texture coordinates
    if(mesh->mTextureCoords[0]) { // Does the mesh contain taexture coords?
      glm::vec2 vec;
      // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
      // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
      vec.x = mesh->mTextureCoords[0][vertexIndex].x;
      vec.y = mesh->mTextureCoords[0][vertexIndex].y;
      vertex.texCoords = vec;
    } else {
      vertex.texCoords = glm::vec2(0.0f, 0.0f);
    }
    
    vertices.push_back(&vertex);
  }
  
  // Now walk through each of the mesh's faces (a face is a mesh its triangle)
  // and retrieve the corresponding vertex indices.
  for(GLuint faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
    aiFace face = mesh->mFaces[faceIndex];
    // Retrieve all indices of the face and store them in the indices vector
    for(GLuint index = 0; index < face.mNumIndices; index++) {
      indices.push_back(&face.mIndices[index]);
    }
  }

  // Process material
  if(mesh->mMaterialIndex != -1) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER
    // Same applies to ohter texture as the following list summarizes:
    // Diffuse: texture_difusseN
    // Specular: texture_specularN
    // Normal: texture_normalN

    // 1. Diffuse maps
    std::vector<Texture *> diffuseMaps = this->loadMaterialTextures(material,
								  aiTextureType_DIFFUSE,
								  "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. Specular maps
    std::vector<Texture *> specularMaps = this->loadMaterialTextures(material,
								     aiTextureType_SPECULAR,
								     "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  // Return a mesh object created from the extracted mesh data
  return new Mesh(vertices, indices, textures);
}

std::vector<Texture *> Model::loadMaterialTextures(aiMaterial* material,
						   aiTextureType type,
						   std::string typeName) {
  std::vector<Texture *> textures = {};
  for(GLuint textureIndex = 0; textureIndex < material->GetTextureCount(type); textureIndex++) {
    aiString str;
    GLboolean skip = false;
    material->GetTexture(type, textureIndex, &str);

    for(GLuint i = 0; i < this->mTexturesLoaded.size(); i++) {

      if(this->mTexturesLoaded[i]->path == str) {
	textures.push_back(this->mTexturesLoaded[i]);
	skip = true;
	break;
      }
    }

    if(!skip) {
      Texture texture;
      Texture2D textureLoader;

      texture.id = textureLoader.loadTexture(str.C_Str(), this->mDirectory);
      texture.type = typeName;
      texture.path = str;
      textures.push_back(&texture);
      this->mTexturesLoaded.push_back(&texture);
    }
  }

  return textures;
}

