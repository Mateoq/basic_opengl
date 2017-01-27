// STD
#include <iostream>
#include <memory>
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
#include "Texture.h"
#include "World.h"
#include "Cube.h"
#include "Plane.h"
#include "Constants.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void doMovement();
GLFWwindow* init();

// Global Variables
const int WIDTH = 1024;
const int HEIGHT = 768;

static bool keys[1024] {false};

static bool firstMouseInput = true;

static GLfloat lastX = WIDTH / 2;
static GLfloat lastY = HEIGHT / 2;

// Delta Time
static GLfloat deltaTime = 0.0f;  // Time between current frame and last frame
static GLfloat lastFrame = 0.0f;  // Time of last frame

Game::World world;

int main() {
  // Init core
  auto window = init();
  if (window == nullptr) {
    return -1;
  }
  
  // Set up variables
  // Game world;
  world.window = window;
  world.screenWidth = WIDTH;
  world.screenHeight = HEIGHT;
  world.camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

  // Setup texture loader
  TextureLoader textureLoader;

  // Set up shaders
  Shader shader("../shaders/advanced.vert", "../shaders/advanced.frag");

  // Load textures
  Graphics::Texture metal = {
    textureLoader.loadTexture("../assets/metal.png"),
    "metal",
    TextureType::DIFFUSE
  };

  Graphics::Texture marble = {
    textureLoader.loadTexture("../assets/marble.jpg"),
    "marble",
    TextureType::DIFFUSE
  };

  // Set up shapes
  std::unique_ptr<Graphics::Cube> cube = std::make_unique<Graphics::Cube>();
  cube->setUp(shader);
  cube->setTexture(marble);
  cube->translate(glm::vec3(-1.0f, 0.0f, -1.0f));

  std::unique_ptr<Graphics::Cube> cube2 = std::make_unique<Graphics::Cube>();
  cube2->setUp(shader);
  cube2->setTexture(marble);
  cube2->translate(glm::vec3(2.0f, 0.0f, 0.0f));
  
  std::unique_ptr<Graphics::Plane> plane = std::make_unique<Graphics::Plane>();
  plane->setUp(shader);
  plane->setTexture(metal);
  
  // Game loop
  while(!glfwWindowShouldClose(world.window)) {
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


    // Render Graphics
    cube->update(world);
    cube->render();

    cube2->update(world);
    cube2->render();

    plane->update(world);
    plane->render();
    
    // Swap the buffers
    glfwSwapBuffers(world.window);
  }

  // Properly de-allocate all resources once they've outlived their purpose    
  glfwTerminate();
  return 0;
}

GLFWwindow* init() {
  if(!glfwInit()) {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return nullptr;
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
    return nullptr;
  }
  glfwMakeContextCurrent(window);

  // =====================================
  // Set the required callback functions
  // =====================================

  // Key input
  glfwSetKeyCallback(window, keyCallback);

  // Mouse input
  glfwSetCursorPosCallback(window, mouseCallback);

  // Scroll input
  glfwSetScrollCallback(window, scrollCallback);

  // GLFW options
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Initialize GLAD to setup the OpenGL Function pointers
  if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return nullptr;
  }

  // Set Viewport size
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  // Setup OpenGL options
  glEnable(GL_DEPTH_TEST);
  // Set depth test func to always, is the same as disable
  glDepthFunc(GL_ALWAYS);

  return window;
}

void doMovement() {
  // Moving the camera
  if(keys[GLFW_KEY_W]) {
    world.camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
  }
  if(keys[GLFW_KEY_S]) {
    world.camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
  }
  if(keys[GLFW_KEY_A]) {
    world.camera.processKeyboard(CameraMovement::LEFT, deltaTime);
  }
  if(keys[GLFW_KEY_D]) {
    world.camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
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

  world.camera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
  world.camera.processMouseScroll(yOffset);
}

