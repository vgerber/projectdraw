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
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateCameraVectors();
}
// Constructor with scalar values
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front_vector(glm::vec3(0.0f, 0.0f, -1.0f))
{
	this->position = glm::vec3(posX, posY, posZ);
	this->world_up_vector = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateCameraVectors();
}

glm::vec3 Camera::getRotation()
{
	return glm::vec3(pitch, yaw, roll);
}

void Camera::rotate(glm::vec3 rotation)
{
	rotate(rotation.x, rotation.y, rotation.z);
}

void Camera::rotate(float pitch, float yaw, float roll)
{
	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
}


// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(this->position, this->position + this->front_vector, this->up_vector);
	//glm::vec3 pos = glm::vec3(10.0f, 5.0f, +10.0f);
	//return glm::lookAt(pos, pos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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
	glm::vec3 front_default = glm::vec3(0.0f, 0.0f, 0.0f);
	target.z *= -1.0f;
	glm::vec3 front_diff = glm::normalize(front_default - target);

	pitch = -glm::degrees(glm::asin(front_diff.y));
	yaw = glm::degrees(std::atan2(front_diff.z, front_diff.x));
}



// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
/*
void Camera::Camera::HandleMouseScroll(GLfloat yoffset)
{
	
	yoffset *= MouseSensitivity * 10;
	if (this->FOV >= 10.0f && this->FOV <= 45.0f)
		this->FOV -= yoffset;
	if (this->FOV <= 10.0f)
		this->FOV = 10.0f;
	if (this->FOV >= 45.0f)
		this->FOV = 45.0f;
		
}
*/

void Camera::dispose()
{
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front_vector = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	this->right_vector = glm::normalize(glm::cross(this->front_vector, this->world_up_vector));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->up_vector = glm::normalize(glm::cross(this->right_vector, this->front_vector));
}
