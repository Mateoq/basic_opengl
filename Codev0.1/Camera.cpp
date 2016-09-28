#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _up,
	       GLfloat _yaw, GLfloat _pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)),
					     movementSpeed(SPEED),
					     mouseSensitivity(SENSITIVITY),
					     zoom(ZOOM) {
  this->position = _position;
  this->worldUp = _up;
  this->yaw = _yaw;
  this->pitch = _pitch;
  this->updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
	       GLfloat upX, GLfloat upY, GLfloat upZ,
	       GLfloat _yaw, GLfloat _pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)),
					     movementSpeed(SPEED),
					     mouseSensitivity(SENSITIVITY),
					     zoom(ZOOM) {
  this->position = glm::vec3(posX, posY, posZ);
  this->worldUp = glm::vec3(upX, upY, upZ);
  this->yaw = _yaw;
  this->pitch = _pitch;
  this->updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::processKeyboard(CameraMovement direction, GLfloat deltaTime) {
  GLfloat velocity = this-> movementSpeed * deltaTime;
  if(direction == CameraMovement::FORWARD) {
    this->position += this->front * velocity;
  }

  if(direction == CameraMovement::BACKWARD) {
    this->position -= this->front * velocity;
  }

  if(direction == CameraMovement::LEFT) {
    this->position -= this->right * velocity;
  }

  if(direction == CameraMovement::RIGHT) {
    this->position += this->right * velocity;
  }

  // This keeps the camera on the ground;
  //this->position.y = 0.0f;
}

void Camera::processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch) {
  xOffset *= this->mouseSensitivity;
  yOffset *= this->mouseSensitivity;

  this->yaw += xOffset;
  this->pitch += yOffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if(constrainPitch) {
    if(this->pitch > 89.0f) {
      this->pitch = 89.0f;
    }

    if(this->pitch < -89.0f) {
      this->pitch = -89.0f;
    }
  }

  // Update front, right and up vectors using the updated Eular angles
  this->updateCameraVectors();
}

void Camera::processMouseScroll(GLfloat yOffset) {
  if(this->zoom >= 1.0f && this->zoom <= 45.0f) {
    this->zoom -= yOffset;
  }

  if(this->zoom <= 1.0f) {
    this->zoom = 1.0f;
  }

  if(this->zoom >= 45.0f) {
    this->zoom = 45.0f;
  }
}

void Camera::updateCameraVectors() {
  // Calculate the new front vector
  glm::vec3 newFront;
  newFront.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
  newFront.y = sin(glm::radians(this->pitch));
  newFront.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
  this->front = glm::normalize(newFront);

  // Also re-calculate the right and up vector
  // Normalize the vectors, because their length gets closer to 0 the more you look up or
  // down which results in slower movement
  this->right = glm::normalize(glm::cross(this->front, this->worldUp));
  this->up = glm::normalize(glm::cross(this->right, this->front));
}

