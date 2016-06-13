#include <iostream>
#include <math.h>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#define WIDTH 800
#define HEIGHT 600

#define LOG_SIZE 512

#define VERTICES_ATTRIB_INDEX 0
#define TEXTURE_ATTRIB_INDEX 1

#define PI 3.14159265359

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
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
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
  
  // Build and compile shader program
  Shader shader("../Shaders/shader.vert", "../Shaders/shader.frag");

  // Set up vertex data (and buffers) and attribute pointers
  GLfloat vertices[] = {
    // Vertices           // Tex Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  // Create VAO, VBO, EBO
  GLuint vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
 
  // Bind VAO
  glBindVertexArray(vao);

  // Copy vertices array in a buffer for OpenGl to use
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Set the attributes pointers
  // ===============================

  // Vertices attribute
  glVertexAttribPointer(VERTICES_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(VERTICES_ATTRIB_INDEX);

  // Texture attribute
  glVertexAttribPointer(TEXTURE_ATTRIB_INDEX, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(TEXTURE_ATTRIB_INDEX);
  
  // Unbind VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbind the VAO
  glBindVertexArray(0);

  // Load and create textures
  // ===========================
  // Texture 1 
  // ===========================
  GLuint textureId1;
  glGenTextures(1, &textureId1);
  glBindTexture(GL_TEXTURE_2D, textureId1);

  // Set the texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Set the texture filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  
  // Load, create texture and generate mipmaps
  int imageWidth, imageHeight;
  std::vector<unsigned char> buffer;
  Texture texture;
  texture.load(buffer, imageWidth, imageHeight, "../Assets/container.png");

  // Image buffer
  GLubyte* image = &buffer[0];
    
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  
  // Free resources
  glBindTexture(GL_TEXTURE_2D, 0);

  // Cube Positions
  glm::vec3 cubePosition[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
  };

  // ===========================
  // Texture 2
  // ===========================
  GLuint textureId2;
  glGenTextures(1, &textureId2);
  glBindTexture(GL_TEXTURE_2D, textureId2);

  // Set the texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Set the texture filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Load, create texture and generate mipmaps
  texture.load(buffer, imageWidth, imageHeight, "../Assets/awesomeface.png");
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Free resources
  glBindTexture(GL_TEXTURE_2D, 0);
  
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activate shader
    shader.use();

    // Bind textures using texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    glUniform1i(glGetUniformLocation(shader.program, "fTexture1"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId2);
    glUniform1i(glGetUniformLocation(shader.program, "fTexture2"), 1);

    // ===========================
    // Coordinate Systems
    // ===========================

    // 3D
    // ===========

    // ===========================
    // Camera
    // ===========================
    
    // View Matrix
    GLfloat radius = 10.0f;
    GLfloat camx = sin(glfwGetTime()) * radius;
    GLfloat camz = cos(glfwGetTime()) * radius;
    glm::mat4 view;
    // Translating the scene in the reverse direction of where we want to move
    view = camera.getViewMatrix();

    // Projection Matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.zoom), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    // Pass uniform model
    GLuint modelLoc = glGetUniformLocation(shader.program, "model");
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Pass uniform view
    GLuint viewLoc = glGetUniformLocation(shader.program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // Pass uniform projection
    GLuint projectionLoc = glGetUniformLocation(shader.program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Draw Container
    glBindVertexArray(vao);

    // Draw the Cubes
    for(GLuint i = 0; i < 10; i++) {
      // Model Matrix
      glm::mat4 model;
      model = glm::translate(model, cubePosition[i]);
      if((i % 3) == 0) {
	// Rotate Cube through time
	model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
      } else {
	// Rotate cube
	GLfloat angle = glm::radians(20.0f * i);
	model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
      }
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      
      glDrawArrays(GL_TRIANGLES, 0, 36); 
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);

    shader.unuse();
    
    // Swap the buffers
    glfwSwapBuffers(window);
  }

  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
    
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

