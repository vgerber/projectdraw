#include "camera.h"
#include <iostream>


Camera::Camera(glm::vec3 position)
{
	this->position = position;
}

Camera::~Camera()
{
}

void Camera::setPosition(glm::vec3 position)
{	
	updateCameraVectors();
	this->position = position;
}


// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : front_vector(glm::vec3(0.0f, 0.0f, -1.0f))
{
	this->position = position;
	this->world_up_vector = up;
	this->updateCameraVectors();
}
// Constructor with scalar values
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front_vector(glm::vec3(0.0f, 0.0f, -1.0f))
{
	this->position = glm::vec3(posX, posY, posZ);
	this->world_up_vector = glm::vec3(upX, upY, upZ);
	this->updateCameraVectors();
}


// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(this->position, this->position + this->front_vector, this->up_vector);
}


glm::vec3 Camera::getFront()
{
	return front_vector;
}

glm::vec3 Camera::getRight()
{
	return right_vector;
}

glm::vec3 Camera::getUp()
{
	return up_vector;
}

void Camera::lookAt(glm::vec3 target)
{
	glm::vec3 front_default = position;
	glm::vec3 front_diff = glm::normalize(target - front_default);
	
	front_vector = front_diff;
	updateCameraVectors();
}

void Camera::dispose()
{
}

void Camera::updateCameraVectors()
{
	// Also re-calculate the Right and Up vector
	this->right_vector = glm::normalize(glm::cross(this->front_vector, this->world_up_vector));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->up_vector = glm::normalize(glm::cross(this->right_vector, this->front_vector));
}
