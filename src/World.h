#pragma once

// STD
#include <memory>

// GLFW
#include <GLFW/glfw3.h>

#include "Camera.h"

namespace Game {
  struct World {
    World() {};
    int screenWidth, screenHeight;
    GLFWwindow* window;
    Camera camera;
  };
}

