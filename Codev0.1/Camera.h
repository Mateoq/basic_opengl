#pragma once

// STD includes
#include <vector>

// GL includes
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction
// to stay away from window-system specific input methods
enum class CameraMovement {
  FORWARD, BACKWARD, LEFT, RIGHT
};

// Default camera values
static GLfloat YAW = 0.0f;
static GLfloat PITCH = 0.0f;
static GLfloat SPEED = 5.0f;
static GLfloat SENSITIVITY = 0.05f;
static GLfloat ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding
// Eular Angles, Vectors and Matrices for use in OpenGL
class Camera {
 public:
  //Constructor with vectors
  Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f),
	 glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f),
	 GLfloat _yaw = YAW, GLfloat _pitch = PITCH);

  // Constructor with scalar values
  Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
	 GLfloat upX, GLfloat upY, GLfloat upZ,
	 GLfloat _yaw, GLfloat _pitch);

  // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
  glm::mat4 getViewMatrix();

  // Processes input received from any keyboard-like input system. Accepts input
  // parameter in the form of camera defined ENUM (to abstract it drom windowing systems)
  void processKeyboard(CameraMovement direction, GLfloat deltaTime);

  // Processes input received from a mouse input system. Expects the offset value in both
  // the x and y direction
  void processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true);

  // Processes input received from a mouse scroll-wheel event. Only requires input
  // on the vertical wheel-axis
  void processMouseScroll(GLfloat yOffset);

  // Camera Attributes
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;
  // Eular Angles
  GLfloat yaw;
  GLfloat pitch;
  // Camera Options
  GLfloat movementSpeed;
  GLfloat mouseSensitivity;
  GLfloat zoom;

 private:
  // Calculates the front vector from the Camera's (updated) Eular Angles
  void updateCameraVectors();
};

