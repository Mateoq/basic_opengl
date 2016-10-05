#pragma once

// STL
#include <stdint.h>
#include <string>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace NoirEngine {
  namespace Graphics {
    typedef uint8_t uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;

    typedef int8_t int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;
    typedef int32 bool32;
    
    class enum TextureType : uint8 { DIFFUSE_TEXTURE = 1, SPECULAR_TEXTURE = 2 };
    
    struct Vertex {
      glm::vec3 position;
      glm::vec3 normals;
      glm::vec2 uv;
    };
    
    struct Texture {
      uint32 id;
      std::string name;
      float width;
      float height;
      TextureType type;
    };
  }
}

