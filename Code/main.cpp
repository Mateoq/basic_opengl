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

static int WIDTH = 1024;
static int HEIGHT = 768;

static const int LOG_SIZE = 512;

static int VERTICES_ATTRIB_INDEX = 0;
static int NORMAL_ATTRIB_INDEX = 1;
static int TEXTURE_ATTRIB_INDEX = 2;

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
  Shader lightShader("../Shaders/lightShader.vert", "../Shaders/lightShader.frag");
  // Light object shaders
  Shader lightObjectShader("../Shaders/lightObjectShader.vert", "../Shaders/lightObjectShader.frag");

  // Set up vertex data (and buffers) and attribute pointers
  GLfloat vertices[] = {
   // Positions           // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
  };

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
  GLuint containerVAO, vbo;
  glGenVertexArrays(1, &containerVAO);
  glGenBuffers(1, &vbo);

  // Copy vertices array in a buffer for OpenGl to use
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Bind VAO
  glBindVertexArray(containerVAO);
  // Set the attributes pointers
  // ===============================

  // Vertices attribute
  glVertexAttribPointer(VERTICES_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(VERTICES_ATTRIB_INDEX);

  // Normal attribute
  glVertexAttribPointer(NORMAL_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(NORMAL_ATTRIB_INDEX);
  
  // Texture attribute
  glVertexAttribPointer(TEXTURE_ATTRIB_INDEX, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(TEXTURE_ATTRIB_INDEX);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // Unbind the VAO
  glBindVertexArray(0);

  // Then, set the light's VAO (VBO stays the same)
  //===========================
  GLuint lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  // Only need to bind the VBO, the containe's VBO's data already contains the correct data.
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // Set the vertex attributes (only position data for the lamp)
  glVertexAttribPointer(VERTICES_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(VERTICES_ATTRIB_INDEX);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Load and create textures
  // ===========================
  // Diffuse Map
  // ===========================
  GLuint diffuseMap;
  glGenTextures(1, &diffuseMap);
  glBindTexture(GL_TEXTURE_2D, diffuseMap);

  // Load, create texture and generate mipmaps
  int imageWidth, imageHeight;
  std::vector<unsigned char> buffer;
  Texture texture;
  texture.load(buffer, imageWidth, imageHeight, "../Assets/container2.png");

  // Image buffer
  GLubyte* image = &buffer[0];
    
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Set texture map parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Set the texture filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  
  // Free resources
  glBindTexture(GL_TEXTURE_2D, 0);

  // ===========================
  // Specular Map
  // ===========================
  GLuint specularMap;
  glGenTextures(1, &specularMap);
  glBindTexture(GL_TEXTURE_2D, specularMap);

  // Load, create texture and generate mipmaps
  texture.load(buffer, imageWidth, imageHeight, "../Assets/container2_specular.png");

  image = &buffer[0];
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Set texture map parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Set the texture filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

  glBindTexture(GL_TEXTURE_2D, 0);

  // ===========================
  // Materials
  // ===========================
  GLint materialDiffuseLoc = glGetUniformLocation(lightShader.program, "material.diffuse");
  GLint materialSpecularLoc = glGetUniformLocation(lightShader.program, "material.specular");

  // Set texture units
  glUniform1i(materialDiffuseLoc, 0);
  glUniform1i(materialSpecularLoc, 1);
  
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
    lightShader.use();

    GLint materialShineLoc = glGetUniformLocation(lightShader.program, "material.shininess");
    glUniform1f(materialShineLoc, 32.0f);

    // ===========================
    // HORROR SCENE
    // ===========================
    
    // ===========================
    // Light
    // ===========================

    // Directional Light
    glUniform3f(glGetUniformLocation(lightShader.program, "directionLight.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(glGetUniformLocation(lightShader.program, "directionLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(lightShader.program, "directionLight.diffuse"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(lightShader.program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);

    // Point Lights
    for(GLuint lightIndex = 0; lightIndex < 4; lightIndex++) {
      glUniform3f(glGetUniformLocation(lightShader.program, ("pointLights[" + std::to_string(lightIndex) + "].positon").c_str()),
		  pointLightPositions[lightIndex].x,
		  pointLightPositions[lightIndex].y,
		  pointLightPositions[lightIndex].z);
      glUniform3f(glGetUniformLocation(lightShader.program, ("pointLights[" + std::to_string(lightIndex) + "].ambient").c_str()),
		  pointLightColors[lightIndex].color.x * 0.1f,
		  pointLightColors[lightIndex].color.y * 0.1f,
		  pointLightColors[lightIndex].color.z * 0.1f);
      glUniform3f(glGetUniformLocation(lightShader.program, ("pointLights[" + std::to_string(lightIndex) + "].diffuse").c_str()),
		  pointLightColors[lightIndex].color.x,
		  pointLightColors[lightIndex].color.y,
		  pointLightColors[lightIndex].color.z);
      glUniform3f(glGetUniformLocation(lightShader.program, ("pointLights[" + std::to_string(lightIndex) + "].specular").c_str()),
		  pointLightColors[lightIndex].color.x,
		  pointLightColors[lightIndex].color.y,
		  pointLightColors[lightIndex].color.z);
      glUniform1f(glGetUniformLocation(lightShader.program, ("pointLights[" + std::to_string(lightIndex) + "].constant").c_str()),
		  pointLightColors[lightIndex].constant);
      glUniform1f(glGetUniformLocation(lightShader.program, ("pointLights[" + std::to_string(lightIndex) + "].linear").c_str()),
		  pointLightColors[lightIndex].linear);
      glUniform1f(glGetUniformLocation(lightShader.program, ("pointLights[" + std::to_string(lightIndex) + "].quadratic").c_str()),
		  pointLightColors[lightIndex].quadratic);
    }

    // Spot Light
    glUniform3f(glGetUniformLocation(lightShader.program, "spotLight.position"),
		camera.position.x, camera.position.y, camera.position.z);
    glUniform3f(glGetUniformLocation(lightShader.program, "spotLight.direction"),
		camera.front.x, camera.front.y, camera.front.z);
    glUniform1f(glGetUniformLocation(lightShader.program, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
    // Soft edges
    glUniform1f(glGetUniformLocation(lightShader.program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
    glUniform3f(glGetUniformLocation(lightShader.program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(lightShader.program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(lightShader.program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(lightShader.program, "spotLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(lightShader.program, "spotLight.linear"), 0.09f);
    glUniform1f(glGetUniformLocation(lightShader.program, "spotLight.quadratic"), 0.032f);
      
    // View position
    GLint viewPosLoc = glGetUniformLocation(lightShader.program, "viewPos");
    glUniform3f(viewPosLoc, camera.position.x, camera.position.y, camera.position.z);
    
    // View Matrix
    glm::mat4 view = camera.getViewMatrix();

    // Projection Matrix
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    // Pass uniform model
    GLuint modelLoc = glGetUniformLocation(lightShader.program, "model");

    // Pass uniform view
    GLuint viewLoc = glGetUniformLocation(lightShader.program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // Pass uniform projection
    GLuint projectionLoc = glGetUniformLocation(lightShader.program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    // Specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // Draw Container
    glm::mat4 model;
    glBindVertexArray(containerVAO);

    for(GLuint i = 0; i < 10; i++) {
      model = glm::mat4();
      model = glm::translate(model, cubePosition[i]);
      GLfloat angle = glm::radians(20.0f * i);
      model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      
      glDrawArrays(GL_TRIANGLES, 0, 36); 
    } 
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
    // A light object is a bit useless  with a directional light since it has no origin
    // Draw the light object
    // ======================
    lightObjectShader.use();

    // Get the location objects for the matrices in the light object
    modelLoc = glGetUniformLocation(lightObjectShader.program, "model");
    viewLoc = glGetUniformLocation(lightObjectShader.program, "view");
    projectionLoc = glGetUniformLocation(lightObjectShader.program, "projection");

    // Set matrices
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(lightVAO);

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
    lightShader.unuse();
    
    // Swap the buffers
    glfwSwapBuffers(window);
  }

  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &containerVAO);
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

