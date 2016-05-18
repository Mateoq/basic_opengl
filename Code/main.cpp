// C++ Headers
#include <iostream>
#include <string>

// OpenGL / SDL / glad Headers
#include <SDL/SDL.h>
#include <glad/glad.h>

#include "Shader.h"

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

std::string gameName = "OpenGL Game";

// SDL Window
SDL_Window* mainWindow;

// OpenGl Context
SDL_GLContext mainContext;

// Object has 4 vertices
const uint32 points = 4;

// Each point has three values (x, y, z)
const uint32 floatsPerPoint = 3;

// Each color has 4 values (r, g, b)
const uint32 floatsPerColor = 4;

// This is the object to draw, a square
static GLfloat square[points][floatsPerPoint] = {
  {-0.5, 0.5, 0.5}, // Top left
  {0.5, 0.5, 0.5}, // Top right
  {0.5, -0.5, 0.5}, // Bottom right
  {-0.5, -0.5, 0.5} // Bottom left
};

// This is the object's colors
static GLfloat colors[points][floatsPerColor] = {
  {0.0, 1.0, 0.0, 1.0}, // Top left
  {1.0, 1.0, 0.0, 1.0}, // Top right
  {1.0, 0.0, 0.0, 1.0}, // Bottom right
  {0.0, 0.0, 1.0, 1.0} // Bottom left
};

// Create variables for storing the ID of our VAO and VBO
GLuint vbo[2], vao[1];

// The positions of the vertices and colors data within th VAO
static uint32 positionAttributeIndez = 0;
static uint32 colorAttributeIndex = 1;

// Wrapper to simplify the shader code
Shader shader;

bool init();
bool setOpenGLAttributes();
void render();
bool setupBufferObjects();
void checkSDLError(int line);
void printSDLGLAttributes();
void cleanUp();

int main(int argc, char* args[]) {
  if(!init()) {
    return -1;
  }

  // Clear buffer with black background
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(mainWindow);
  return 0;
}

bool init() {
  // Initialize SDL's Video subsystem
  if(SDL_Init(SDL_INIT_VIDEO)) {
    std::cout << "Failed to init SDL\n";
    return false;
  }

  // Create window
  mainWindow = SDL_CreateWindow(gameName.c_str(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				800, 600,
				SDL_WINDOW_OPENGL);

  // Check if window was created
  if(!mainWindow) {
    std::cout << "Unable to create window\n";
    checkSDLError(__LINE__);
    return false;
  }

  // Create openGL context and attach it to the window
  mainContext = SDL_GL_CreateContext(mainWindow);

  setOpenGLAttributes();

  // Make buffer swap syncronized with monitor's vertical refesh
  SDL_GL_SetSwapInterval(1);

  // Init Glad
  if(!gladLoadGL()){
    std::cout << "glad couldn't be loaded!\n";
    return false;
  }

  return true;
}

bool setOpenGLAttributes() {
  // Set OpenGL version
  // SDL_GL_CONTEXT_CORE gives us only the newer version, depricated functions are disabled
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // 3.2 is part of the mmodern versions of OpenGL, but most video cards would be able to run it
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  // Turn on double buffering with a 24bit Z buffer
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  return true;
}

void render() {
  // First, render a square without any colors (all vertices will be white)
  // =========================================
  // Make background grey
  glClearColor(0.5, 0.5, 0.5, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  //Invoce glDrawArrays telling that the data is a line loop and need to draw 2 - 4 vertices
  glDrawArrays(GL_LINE_LOOP, 0, 4);

  // Swap buffers to make changes visible
  SDL_GL_SwapWindow(mainWindow);

  std::cout << "Press ENTER to render next frame\n";
  std::cin.ignore();

  // Second, enable the colors and draw a solid square
  // =========================================
  // Enable attribute within the current VAO
  glEnableVeertexAttribArray(colorAttributeIndex);

  // Make background white
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  // Invoke glDrawArrays telling that our data is a line loop and need to draw 2 - 4 vertices
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  // Swap our buffers to make our changes visible
  SDL_GL_SwapWindow(mainWindow);
}

bool setupBufferObjects() {
  // Generate and assign two Vertex Buffer Objects to handle
  glGenBuffers(2, vbo);

  // Generate and assign a Vertex Array Object to handle
  glGenVertexArrays(1, vao);

  // Bind our VAO as the current used object
  glBindVertexArray(vao[0]);

  // Positions
  // ===============================
  // Bind the first VBO as being the buffer anf storing vertex attributes (coordinates)
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

  // Copy the vertex data from square to the buffer
  
}

void cleanUp() {
  // Delete OpenGL context
  SDL_GL_DeleteContext(mainContext);

  // Destroy window
  SDL_DestroyWindow(mainWindow);

  // Shutdown SDL 2
  SDL_Quit();
}

void checkSDLError(int line = -1) {
  std::string error = SDL_GetError();

  if(error != "") {
    std::cout << "SDL Error: " << error << std::endl;

    if(line != -1) {
      std::cout << "\nLine: " << line << std::endl;
    }

    SDL_ClearError();
  }
}

void printSDLGLAttributes() {
  int value = 0;
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
  std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION: " << value << std::endl;

  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
  std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}

