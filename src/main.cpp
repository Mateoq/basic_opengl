#include <iostream>
#include <string>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "TextureLoader.h"
#include "Camera.h"
#include "Cube.h"
#include "Plane.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void doMovement();

// Global Variables
const int WIDTH = 1024;
const int HEIGHT = 768;

const bool keys[1024];

int main() {
  if(!glfwInit()) {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return -1;
  }
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Cerate a GLFWwindow object that we can use for GLFW's functions
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Game", nullptr, nullptr);
  if(window == nullptr) {
    std::cout << "Failed to create  GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // =====================================
  // Set the required callback functions
  // =====================================

  // Key input
  glfwSetKeyCallback(window, keyCallback);

  // Mouse input
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallback);

  // Scroll input
  glfwSetScrollCallback(window, scrollCallback);

  // Initialize GLAD to setup the OpenGL Function pointers
  if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Set Viewport size
  glViewport(0, 0, WIDTH, HEIGHT);

  // Setup OpenGL options
  glEnable(GL_DEPTH_TEST);
  // Set depth test func to always, is the same as disable
  glDepthFunc(GL_ALWAYS);

  // Set up variables
  // Camera
  const Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
  const bool firstMouseInput = true;

  const GLfloat lastX = WIDTH / 2;
  const GLfloat lastY = HEIGHT / 2;

  // Delta Time
  const GLfloat deltaTime = 0.0f;  // Time between current frame and last frame
  const GLfloat lastFrame = 0.0f;  // Time of last frame

  // Setup texture loader
  TextureLoader textureLoader();

  // Set up shaders
  Shader shader("../shaders/advanced.vert", "../shaders/advanced.frag");

  // Set up shapes
  Cube cube;
  Plane plane;
  
  // Game loop
  while(!glfwWindowShouldClose(window)) {
    // Calculate average time of the frame
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    // Check and call events
    glfwPollEvents();
    doMovement();

    // Render
    // Clear the color buffer
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Swap the buffers
    glfwSwapBuffers(window);
  }

  // Properly de-allocate all resources once they've outlived their purpose    
  glfwTerminate();
  return 0;
}

void doMovement() {
  // Moving the camera
  if(keys[GLFW_KEY_W]) {
    camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
  }
  if(keys[GLFW_KEY_S]) {
    camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
  }
  if(keys[GLFW_KEY_A]) {
    camera.processKeyboard(CameraMovement::LEFT, deltaTime);
  }
  if(keys[GLFW_KEY_D]) {
    camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
  }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  // When a user presses the escape key, we set the WindowShouldClose property to true,
  // closing the application
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if(key >= 0 && key <= 1024) {
    if(action == GLFW_PRESS) {
      keys[key] = true;
    } else if(action == GLFW_RELEASE) {
      keys[key] = false;
    }
  }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
  if(firstMouseInput) {
    lastX = xpos;
    lastY = ypos;
    firstMouseInput = false;
  }
  
  GLfloat xOffset = xpos - lastX;
  GLfloat yOffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
  lastX = xpos;
  lastY = ypos;

  camera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
  camera.processMouseScroll(yOffset);
}

