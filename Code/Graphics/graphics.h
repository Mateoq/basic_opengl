#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace NoirEngine {
  namespace Graphics {
    
    struct Vertex {
      glm::vec3 position;
      glm::vec3 normals;
      glm::vec2 uv;
    };
    
    struct Texture {
      
    };
    
  }
}

