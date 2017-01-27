#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <glad/glad.h>
#include <FreeImagePlus.h>

class TextureLoader {
 public:
  TextureLoader();

  GLuint loadTexture(const std::string imagePath);

 private:
  GLuint setupGLTexture(std::unique_ptr<GLubyte>  textureData,
			std::string textureName,
			int witdh, int height);
  
  bool validateGLTexture(GLenum error, std::string textureName);
};

