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
  
  // Light shaders
  Shader modelShader("../Shaders/modelLoading.vert", "../Shaders/modelLoading.frag");
  // Light object shaders
  Shader lightObjectShader("../Shaders/lightObjectShader.vert", "../Shaders/lightObjectShader.frag");

  // Load models
  Model sampleModel("../Assets/Models/Nanosuit/nanosuit.obj");

  // Set up vertex data (and buffers) and attribute pointers
  GLfloat vertices[] = {
   // Positions           
    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,   
     0.5f,  0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f, 
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 

    -0.5f, -0.5f,  0.5f, 
     0.5f, -0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f, 
    -0.5f, -0.5f,  0.5f, 

    -0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f, 

     0.5f,  0.5f,  0.5f, 
     0.5f,  0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f, 

    -0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f,  0.5f, 
     0.5f, -0.5f,  0.5f, 
    -0.5f, -0.5f,  0.5f, 
    -0.5f, -0.5f, -0.5f, 

    -0.5f,  0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f, 
     0.5f,  0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f, -0.5f
  };

  // Light objects positions
  glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f),
    glm::vec3(0.0f, 0.0f, -3.0f)
  };

  // Light colors
  // =====================================
  // Horror
  Light pointLightColors[] = {
    Light({ glm::vec3(0.1f, 0.1f, 0.1f), 1.0f, 0.14f, 0.07f }),
    Light({ glm::vec3(0.1f, 0.1f, 0.1f), 1.0f, 0.14f, 0.07f }),
    Light({ glm::vec3(0.1f, 0.1f, 0.1f), 1.0f, 0.22f, 0.20f }),
    Light({ glm::vec3(0.3f, 0.1f, 0.1f), 1.0f, 0.14f, 0.07f })
  };

  // First set the container's VAO (and VBO)
  //======================================
  GLuint vbo;
  glGenBuffers(1, &vbo);

  // Then, set the light's VAO (VBO stays the same)
  //===========================
  GLuint lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  // Only need to bind the VBO, the containe's VBO's data already contains the correct data.
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // Set the vertex attributes (only position data for the lamp)
  glVertexAttribPointer(VERTEX_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(VERTEX_ATTRIB_INDEX);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
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
    modelShader.use();

    // ===========================
    // Light
    // ===========================

    // Directional Light
    glUniform3f(glGetUniformLocation(modelShader.program, "directionLight.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(glGetUniformLocation(modelShader.program, "directionLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(modelShader.program, "directionLight.diffuse"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(modelShader.program, "directionLight.specular"), 0.2f, 0.2f, 0.2f);

    // Point Lights
    for(GLuint lightIndex = 0; lightIndex < 4; lightIndex++) {
      glUniform3f(glGetUniformLocation(modelShader.program,
				       ("pointLights[" + std::to_string(lightIndex) + "].positon").c_str()),
		  pointLightPositions[lightIndex].x,
		  pointLightPositions[lightIndex].y,
		  pointLightPositions[lightIndex].z);
      glUniform3f(glGetUniformLocation(modelShader.program,
				       ("pointLights[" + std::to_string(lightIndex) + "].ambient").c_str()),
		  pointLightColors[lightIndex].color.x * 0.1f,
		  pointLightColors[lightIndex].color.y * 0.1f,
		  pointLightColors[lightIndex].color.z * 0.1f);
      glUniform3f(glGetUniformLocation(modelShader.program,
				       ("pointLights[" + std::to_string(lightIndex) + "].diffuse").c_str()),
		  pointLightColors[lightIndex].color.x,
		  pointLightColors[lightIndex].color.y,
		  pointLightColors[lightIndex].color.z);
      glUniform3f(glGetUniformLocation(modelShader.program,
				       ("pointLights[" + std::to_string(lightIndex) + "].specular").c_str()),
		  pointLightColors[lightIndex].color.x,
		  pointLightColors[lightIndex].color.y,
		  pointLightColors[lightIndex].color.z);
      glUniform1f(glGetUniformLocation(modelShader.program,
				       ("pointLights[" + std::to_string(lightIndex) + "].constant").c_str()),
		  pointLightColors[lightIndex].constant);
      glUniform1f(glGetUniformLocation(modelShader.program,
				       ("pointLights[" + std::to_string(lightIndex) + "].linear").c_str()),
		  pointLightColors[lightIndex].linear);
      glUniform1f(glGetUniformLocation(modelShader.program,
				       ("pointLights[" + std::to_string(lightIndex) + "].quadratic").c_str()),
		  pointLightColors[lightIndex].quadratic);
    }

    // Spot Light
    glUniform3f(glGetUniformLocation(modelShader.program, "spotLight.position"),
		camera.position.x, camera.position.y, camera.position.z);
    glUniform3f(glGetUniformLocation(modelShader.program, "spotLight.direction"),
		camera.front.x, camera.front.y, camera.front.z);
    glUniform1f(glGetUniformLocation(modelShader.program, "spotLight.cutOff"),
		glm::cos(glm::radians(10.0f)));
    // Soft edges
    glUniform1f(glGetUniformLocation(modelShader.program, "spotLight.outerCutOff"),
		glm::cos(glm::radians(15.0f)));
    glUniform3f(glGetUniformLocation(modelShader.program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(modelShader.program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(modelShader.program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(modelShader.program, "spotLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(modelShader.program, "spotLight.linear"), 0.09f);
    glUniform1f(glGetUniformLocation(modelShader.program, "spotLight.quadratic"), 0.032f);
      
    // View position
    GLint viewPosLoc = glGetUniformLocation(modelShader.program, "viewPos");
    glUniform3f(viewPosLoc, camera.position.x, camera.position.y, camera.position.z);
    
    // Transormation matrices
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom),
					    (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(modelShader.program, "projection"),
		       1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(modelShader.program, "view"),
		       1, GL_FALSE, glm::value_ptr(view));

    // Draw the loaded model
    sampleModel.draw(&modelShader);
    
    // A light object is a bit useless  with a directional light since it has no origin
    // Draw the light object
    // ======================
    lightObjectShader.use();

    // Get the location objects for the matrices in the light object
    GLuint modelLoc = glGetUniformLocation(lightObjectShader.program, "model");
    GLuint viewLoc = glGetUniformLocation(lightObjectShader.program, "view");
    GLuint projectionLoc = glGetUniformLocation(lightObjectShader.program, "projection");

    // Set matrices
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(lightVAO);

    glm::mat4 model;
    for(GLint objectIndex = 0; objectIndex < 4; objectIndex++) {
      model = glm::mat4();
      model = glm::translate(model, pointLightPositions[objectIndex]);
      model = glm::scale(model, glm::vec3(0.2f));
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      // Include light color
      glUniform3f(glGetUniformLocation(lightObjectShader.program, "lightColor"),
		  pointLightColors[objectIndex].color.x,
		  pointLightColors[objectIndex].color.y,
		  pointLightColors[objectIndex].color.z);

      glDrawArrays(GL_TRIANGLES, 0, 36); 
    }

    glBindVertexArray(0);
    
    lightObjectShader.unuse();
    modelShader.unuse();
    
    // Swap the buffers
    glfwSwapBuffers(window);
  }

  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &lightVAO);
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

