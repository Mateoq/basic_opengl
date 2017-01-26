#pragma once

// STD
#include <string>

// GLAD
#include <glad/glad.h>

#include "Constants.h"

namespace Graphics {
  struct Texture {
    GLuint id;
    std::string name;
    TextureType type;
  };
}

