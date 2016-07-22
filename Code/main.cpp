#include <iostream>
#include <math.h>
#include <string>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Assimp
#include <assimp/Importer.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Model.h"

static int WIDTH = 1024;
static int HEIGHT = 768;

static const int LOG_SIZE = 512;

static float PI = 3.14159265359;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void doMovement();

// Global Variables
static bool keys[1024];

// Camera
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static bool firstMouseInput = true;

static GLfloat lastX = WIDTH / 2;
static GLfloat lastY = HEIGHT / 2;

static GLfloat deltaTime = 0.0f;  // Time between current frame and last frame
static GLfloat lastFrame = 0.0f;  // Time of last frame

static glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);

struct Light {
  glm::vec3 color;
  float constant;
  float linear;
  float quadratic;
};

int main() {
  Assimp::Importer importer;
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
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
  if(window == nullptr) {
    std::cout << "Failed to create  GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // =====================================
  // Set the required callback funcitons
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
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  // Define the viewport dimesions
  glViewport(0, 0, width, height);

  // Setup OpenGL options
  glEnable(GL_DEPTH_TEST);
  
  // Light shaders
  Shader modelShader("../Shaders/modelLoading.vert", "../Shaders/modelLoading.frag");

  // Load models
  Model sampleModel("../Assets/Models/Nanosuit/nanosuit2.obj");
  
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

    // Activate shader
    modelShader.use();
    
    // Transormation matrices
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom),
					    (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(modelShader.program, "projection"),
		       1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(modelShader.program, "view"),
		       1, GL_FALSE, glm::value_ptr(view));

    // Draw the loaded model
    glm::mat4 model;
    // Translate it down a bit so it's at the center of the scene
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
    // It's a bit too big for the scene, so scale it down
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(glGetUniformLocation(modelShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    sampleModel.draw(&modelShader);
    
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

