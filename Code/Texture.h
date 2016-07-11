#pragma once

#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

// GLAD
#include <glad/glad.h>

class Texture2D {
 public:
  // Load Texture
  GLuint loadTexture(const GLchar* filename, const std::string directory);
 
 private:
  std::vector<unsigned char> buffer;

   // Load file
  int load(std::vector<unsigned char>& outImage, int& imageWidth, int& imageHeight, const char* filename);

  // decodePNG
  int decodeFile(std::vector<unsigned char>& out_image, int& image_width, int& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);
};
