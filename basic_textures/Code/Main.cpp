#include <iostream>
#include <math.h>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Texture.h"

#define WIDTH 800
#define HEIGHT 600

#define LOG_SIZE 512

#define VERTICES_ATTRIB_INDEX 0
#define COLOR_ATTRIB_INDEX 1
#define TEXTURE_ATTRIB_INDEX 2

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void drawTriangle();
void drawSquare();

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

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
  if(window == nullptr) {
    std::cout << "Failed to create  GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Set Viewport size
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  glfwSetKeyCallback(window, keyCallback);
  
  // Setup shader program
  Shader shader("../Shaders/shader.vert", "../Shaders/shader.frag");

  // Triangle vertices
  GLfloat vertices[] = {
    // Position         // Colors          // Texture Coords
    0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // Top Right
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,   // Bottom Left
    -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f   // Top Left
  };
  
  GLuint indices[] = {
    0, 1, 3, // First Triangle
    1, 2, 3  // Second Triangle
  };

  // Create VAO, VBO, EBO
  GLuint vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  // Bind VAO
  glBindVertexArray(vao);

  // Copy vertices array in a buffer for OpenGl to use
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Copy the index array in a element buffer for OpenGL to use
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Set the attributes pointers

  // Vertices attribute
  glVertexAttribPointer(VERTICES_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(VERTICES_ATTRIB_INDEX);

  // Color attribute
  glVertexAttribPointer(COLOR_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(COLOR_ATTRIB_INDEX);

  // Texture attribute
  glVertexAttribPointer(TEXTURE_ATTRIB_INDEX, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(TEXTURE_ATTRIB_INDEX);
  
  // Unbind VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbind EBO
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
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
    // Check and call events
    glfwPollEvents();

    // Render
    // Clear the color buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Activate shader
    shader.use();

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    glUniform1i(glGetUniformLocation(shader.program, "fTexture1"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId2);
    glUniform1i(glGetUniformLocation(shader.program, "fTexture2"), 1);
    

    // Draw triangle
    glBindVertexArray(vao);

    //  glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  // When a user presses the escape key, we set the WindowShouldClose property to true,
  // closing the application
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

