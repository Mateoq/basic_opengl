#include "Shader.h"

bool Shader::init() {
  // Generate our shader. Similar to glGenBuffers() and glGenVertezArray(), except that this returns the ID
  shaderProgram = glCreateProgram();

  // Bind the location of the attributes
  bindAttributeLocation(0, "in_Position");
  bindAttributeLocation(1, "in_Color");

  if(!loadShader("shader.vert"), GL_VERTEX_SHADER) {
    return false;
  }

  if(!loadShader("shader.geom"), GL_GEOMETRY_SHADER) {
    return false;
  }
  
  if(!loadShader("shader.frag"), GL_FRAGMENT_SHADER) {
    return false;
  }

  // All shaders has been created, now all will be put together into one large object
  return linkShaders();
}

void Shader::bindAttributeLocation(int index, const std::string& attribute) {
  // Bind attribute index 0 (coordinates) to in_Position and index 1 (color) to in_Color
  // Attribute locations must be setup before calling glLinkProgram
  glBindAttribLocation(shaderProgram, index, attribute.c_str());
}

bool Shader::loadShader(const std::string& fileName) {
  std::cout << "Loading Shader: " << fileName << std::endl;

  // Create shader
  int shaderId = createShader(fileName, shaderType);

  if(tryCompileShader(shaderId)) {
    glAttachShader(shaderProgram, shaderId);
    shaderIds.push_back(shaderId);

    return true;
  }

  return false;
}

void Shader::useProgram() {
  // Load the shader into the rendering pipeline
  glUseProgram(shaderProgram);
}

bool Shader::tryCompileShader(GLuint shaderId) {
  // Compile the shader
  glCompileShader(vertexShader);

  GLint wasCompiled = 0;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &wasCompiled);
  
  printShaderCompilationErrorInfo(shaderId);

  // Return false if compilation failed
  return (wasCompiled != GL_FALSE);
}

GLuint Shader::createShader(const std::string& fileName, GLenum shaderType) {
  // Read file as std::string
  std::string str = readFile(filename.c_str());

  const char* src = str.c_str();

  // Create an empty shader handler
  shaderId = glCreateShader(shaderType);

  // Send the shader source code to OpenGL
  glShaderSource(shaderId, 1, &src, nullptr);

  return shaderId;
}

bool Shader::linkShaders() {
  // Link. A this point, the shaders will be inspected/optimized
  // and the binary code generate.
  // The binary code will the be uploaded to the GPU
  glLinkProgram(shaderProgram);

  // Verify that the linking succeded
  int isLinked;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);

  if(!isLinked) {
    printShaderLinkingError();
  }

  return isLinked != 0;
}

std::string Shader::readFile(const char* file) {
  // Open file
  std::ifstream t(file);

  if(t.fail()) {
    perror(file);
    printf("Failed to open %s", file);
  }

  // Read file ino buffer
  std::string fileContents = "";
  std::string line;

  while(std::getline(t, line)) {
    fileContents += line + "\n";
  }

  t.close();

  return fileContents;
}

void Shader::printShaderLinkingError() {
  std::cout << "============================\n";
  std::cout << "Shader linking failed: " << std::endl;

  // Find length of shader info log
  int maxLength;
  glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

  std::cout << "Info Length: " << maxLength << std::endl;

  // Get shader info log
  std::vector<GLchar> shaderProgramInfoLog(maxLength);
  glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &shaderProgramInfoLog[0]);

  std::printf("\tLinker error message: %s\n", &(shaderProgramInfoLog[0]));
  // Handle the error in an appropiate way such as displaying a message
  // or writing to a log file.
  // In this simple program, we'll just leave
  return;
}

void Shader::printShaderCompilationErrorInfo(GLuint shaderId) {
  // Find length of shader info log
  GLint maxLength = 0;
  glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

  // Get shader info log
  std::vector<GLchar> log(maxLength);
  glGetShaderInfoLog(shaderId, maxLength, &maxLength, &log[0]);

  if(log.length()) {
    glDeleteShader(shaderId);
    std::cout << "============================\n";
    std::cout << "Shader compilation failed: " << std::endl;

    // Print shader info log
    std::printf("\tError info: %s\n", &(shaderInfoLog[0]));
    std::cout << "=================================================\n\n";
  }
  
}

void Shader::cleanUp() {
  // Cleanup all the things we bound and allocated
  glUseProgram(0);

  for(GLuint i : shaderIds) {
     glDetachShader(shaderProgram, i);
  }

  glDeleteProgram(shaderProgram);

  for(GLuint i : shaderIds) {
    glDeleteShader(i);
  }
}

