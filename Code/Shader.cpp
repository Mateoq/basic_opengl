#include "Shader.h"

std::string Shader::readFile(const char* file) {
  // Open file
  std::ifstream t(file);

  // Read file ino buffer
  std::stringstream buffer;
  buffer << t.rdbuf();

  // Make a std::string and fill it with the contents of buffer
  std::string fileContent = buffer.str();

  return fileContent;
}

void Shader::bindAttributeLocation(int index, const std::string& attribute) {
  // Bind attribute index 0 (coordinates) to in_Position and index 1 (color) to in_Color
  // Attribute locations must be setup before calling glLinkProgram
  glBindAttribLocation(shaderProgram, index, attribute.c_str());
}

void Shader::useProgram() {
  // Load the shader into the rendering pipeline
  glUseProgram(shaderProgram);
}

bool Shader::init() {
  // Generate our shader. Similar to glGenBuffers() and glGenVertezArray(), except that this returns the ID
  shaderProgram = glCreateProgram();

  // Bind the location of the attributes
  bindAttributeLocation(0, "in_Position");
  bindAttributeLocation(1, "in_Color");

  if(!loadVertexShader("shader.vert")) {
    return false;
  }
  
  if(!loadFragmentShader("shader.frag")) {
    return false;
  }

  // All shaders has been created, now all will be put together into one large object
  return linkShaders();
}

bool Shader::loadVertexShader(const std::string& filename) {
  std::cout << "Linking Vertex Shader" << std::endl;

  // Read file as std::string
  std::string str = readFile(filename.c_str());
  //const char* contentsPtr = str.c_str();
  // c_srt() returns a const char*, convert it to a non-const one
  const char* src = str.c_str();
  int32 size = str.length();

  // Create an empty vertex shader handler
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  // Send the vertex shader source code to OpenGL
  glShaderSource(vertexShader, 1, &src, nullptr);

  // Compile the vertex shader
  glCompileShader(vertexShader);

  int wasCompiled = 0;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &wasCompiled);

  if(wasCompiled == 0) {
    printShaderCompilationErrorInfo(vertexShader);
    return false;
  }

  glAttachShader(shaderProgram, vertexShader);
  return true;
}

bool Shader::loadFragmentShader(const std::string& filename) {
  std::cout << "Loading Fragment Shader" << std::endl;

  // Read file as std::string
  std::string str = readFile(filename.c_str());

  // c_srt() returns a const char*, convert it to a non-const one
  const char* src = str.c_str();
  int32 size = str.length();

  // Create an empty fragment shader handler
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  // Send the fragment shader source code to OpenGL
  glShaderSource(fragmentShader, 1, &src, nullptr);

  // Compile the vertex shader
  glCompileShader(fragmentShader);

  int wasCompiled = 0;

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &wasCompiled);

  if(wasCompiled == 0) {
    printShaderCompilationErrorInfo(fragmentShader);
    return false;
  }

  glAttachShader(shaderProgram, fragmentShader);
  return true;
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
    printShaderLinkingError(shaderProgram);
  }

  return isLinked != 0;
}

void Shader::printShaderLinkingError(int32 shaderId) {
  std::cout << "============================\n";
  std::cout << "Shader linking failed: " << std::endl;

  // Find length of shader info log
  int maxLength;
  glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

  std::cout << "Info Length: " << maxLength << std::endl;

  // Get shader info log
  char* shaderProgramInfoLog = new char[maxLength];
  glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

  std::cout << "Linker error message: " << shaderProgramInfoLog << std::endl;

  // Handle the error in an appropiate way such as displaying a message
  // or writing to a log file.
  // In this simple program, we'll just leave
  delete shaderProgramInfoLog;
  return;
}

void Shader::printShaderCompilationErrorInfo(int32 shaderId) {
  std::cout << "============================\n";
  std::cout << "Shader compilation failed: " << std::endl;

  // Find length of shader info log
  int maxLength;
  glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

  // Get shader info log
  char* shaderInfoLog = new char[maxLength];
  glGetShaderInfoLog(shaderProgram, maxLength, &maxLength, shaderInfoLog);

  // Print shader info log
  std::cout << "\tError info: " << shaderInfoLog << std::endl;
  std::cout << "=================================================\n\n";
  delete shaderInfoLog;
}

void Shader::cleanUp() {
  // Cleanup all the things we bound and allocated
  glUseProgram(0);
  glDetachShader(shaderProgram, vertexShader);
  glDetachShader(shaderProgram, fragmentShader);

  glDeleteProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}
