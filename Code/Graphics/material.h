#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// FreeImage
#include <FreeImagePlus.h>

// Graphics
#include "graphics.h"

namespace NoirEngine {
  namespace Graphics {
    struct MaterialProps {
      uint32 id;
      uint32 specularMap;
      uint32 diffuseMap;
      float shininess;
    };

    class Material {
    public:
      // Constructor
      Material(MaterialProps props);

      // Destructure
      ~Material();
    };
  }
}

