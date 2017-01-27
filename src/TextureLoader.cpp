#include "TextureLoader.h"

TextureLoader::TextureLoader() {
  FreeImage_Initialise(true);
}

GLuint TextureLoader::loadTexture(const std::string imagePath) {
  // Get the filename as a pointer to a const char array
  // to play nice with FreeImage
  const char* filename = imagePath.c_str();
  
  // Determine the format of the image.
  // Note: The second paramter ('size') is currently unused,
  // and we should use 0 for it.
  FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

  // Check if the image was not found
  if (format == -1) {
    std::cout << "Could not found image: " << filename << "!!" << std::endl;
    return -1;
  }

  // Image found, check its format
  if (format == FIF_UNKNOWN) {
    std::cout
      << "Couldn't determine file format - attempting to get from file extension..."
      << std::endl;

    // ...by getting the filetype from the filename extension (i.e. .PNG, .GIF etc.)
    // Note: This is slower and more error-prone that getting it from the file itself,
    // also, we can't use the 'U' (unicode) variant of this method as that's Windows only.
    format = FreeImage_GetFIFFromFilename(filename);

    if (!FreeImage_FIFSupportsReading(format)) {
      std::cout << "Detected image format cannot be read!!" << std::endl;
      return -1;
    }
  }

  // Load image in a bitmap
  FIBITMAP* bitmap = FreeImage_Load(format, filename);

  // Bits-per-pixel from the source image
  int bitsPerPixel = FreeImage_GetBPP(bitmap);

  // Convert our image up to 32 bits (8 bits per channel, Red/Green/Blue/Alpha) -
  // but only if the image is not already 32 bits (i.e. 8 bits per channel).
  // Note: ConvertTo32Bits returns a CLONE of the image data - so if we
  // allocate this back to itself without using our bitmap32 intermediate
  // we will LEAK the original bitmap data, and valgrind will show things like this:
  //
  // LEAK SUMMARY:
  //  definitely lost: 24 bytes in 2 blocks
  //  indirectly lost: 1,024,874 bytes in 14 blocks    <--- Ouch.
  //
  // Using our intermediate and cleaning up the initial bitmap data we get:
  //
  // LEAK SUMMARY:
  //  definitely lost: 16 bytes in 1 blocks
  //  indirectly lost: 176 bytes in 4 blocks
  //
  // All above leaks (192 bytes) are caused by XGetDefault (in /usr/lib/libX11.so.6.3.0) - we have no control over this.
  //

  FIBITMAP* bitmap32;
  if (bitsPerPixel == 32) {
    std::cout
      << "Source image has "
      << bitsPerPixel
      << " bits per pixel. Skipping conversion." << std::endl;
    bitmap32 = bitmap;
  } else {
    std::cout
      << "Source image has "
      << bitsPerPixel
      << " bits per pixel. Converting to 32-bit color." << std::endl;
    bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
  }

  // Get image info
  int imageWidth = FreeImage_GetWidth(bitmap32);
  int imageHeight = FreeImage_GetHeight(bitmap32);
  std::cout << "Image: " << imagePath << std::endl
	    << "Size: " << imageWidth << "x" << imageHeight << std::endl;

  // Get a pointer to the texture data as an array of unsigned bytes.
  std::unique_ptr<GLubyte> textureData(FreeImage_GetBits(bitmap32));

  GLuint textureId = this->setupGLTexture(std::move(textureData), filename, imageWidth, imageHeight);

  // Unload the 32-bit colour bitmap
  FreeImage_Unload(bitmap32);

  if (bitsPerPixel != 32) {
    FreeImage_Unload(bitmap);
  }

  if (textureId == -1) {
    return -1;
  }

  return textureId;
}

GLuint TextureLoader::setupGLTexture(std::unique_ptr<GLubyte> textureData,
				     std::string textureName,
				     int width, int height) {
  //Generate texture ID and load texture data
  GLuint textureId;
  glGenTextures(1, &textureId);

  // Assign texture to ID
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexImage2D(GL_TEXTURE_2D, // Type of texture
		0, // Mipmap level
		GL_RGBA, // Internal format
		width, // Width of the texture
		height, // Height of the texture
		0, // Border in pixels
		GL_RGBA, // Data format
		GL_UNSIGNED_BYTE, // Type of texture data
	       (const void*)textureData.release()); // The image data
  glGenerateMipmap(GL_TEXTURE_2D);
  
  // Parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLenum glError = glGetError();
  glBindTexture(GL_TEXTURE_2D, 0);

  if (!this->validateGLTexture(glError, textureName)) {
    return -1;
  }

  return textureId;
}

bool TextureLoader::validateGLTexture(GLenum error, std::string textureName) {
  if (error) {
    std::cout << "There was an error loading the texture: "
	      << textureName << std::endl;

    switch(error) {
    case GL_INVALID_ENUM:
      std::cout << "= Invalid enum" << std::endl;
      break;
    case GL_INVALID_VALUE:
      std::cout << "= Invalid value" << std::endl;
      break;
    case GL_INVALID_OPERATION:
      std::cout << "= Invalid operation" << std::endl;
      break;
    default:
      std::cout << "Unrecognised GLenum" << std::endl;
      break;
    }
    
    return false;
  }
  return true;
}

