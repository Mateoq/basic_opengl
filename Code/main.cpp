#include <iostream>
#include <math.h>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// SOIL
#include <SOIL/SOIL.h>

#include "Shader.h"

#define WIDTH 800
#define HEIGHT 600

#define LOG_SIZE 512

#define VERTICES_ATTRIB_INDEX 0
#define COLOR_ATTRIB_INDEX 1

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

  // Setup one texture
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // Set the texture wrapping/filtering options(on the currently bound texture object)

  // Load and generate the texture
  int imageWidth, imageHeight;
  GLubyte* image = SOIL_load_image("../Assets/container.jpg", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Free resources
  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  // Setup shader program
  Shader shader("../Shaders/shader.vert", "../Shaders/shader.frag");

  // Triangle vertices
  GLfloat vertices[] = {
    // Position      
    0.0f, 0.5f, 0.0f,  // Top Center
    0.5f, -0.5f, 0.0f,  // Bottom Left
    -0.5f, -0.5f, 0.0f  // Bottom Right
  };

  // Texture coordinates
  GLfloat texCoords[] = {
    0.0f, 0.0f,  // Lower-left corner
    1.0f, 0.0f,  // Lower-right corner
    0.5f, 1.0f   // Top-center corner
  };

  // Create VAO, VBO, EBO
  GLuint vao, vbo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  // Bind VAO
  glBindVertexArray(vao);

  // Copy vertices array in a buffer for OpenGl to use
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Set the attributes pointers
  glVertexAttribPointer(VERTICES_ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(VERTICES_ATTRIB_INDEX);
  
  // Unbind VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  // Unbind the VAO
  glBindVertexArray(0);

#if 0
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
  
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

    // Update the uniform color
    GLfloat timeValue = glfwGetTime();
    GLfloat greenValue = (sin(timeValue) / 2) + 0.5f;
    GLint vertexColorLocation = glGetUniformLocation(shader.program, "xColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    // Draw triangle
    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    //    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

