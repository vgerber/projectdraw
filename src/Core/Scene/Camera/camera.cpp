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

glm::vec3 Position;
glm::vec3 Front;
glm::vec3 Up;
glm::vec3 Right;
glm::vec3 WorldUp;
// Eular Angles
GLfloat Yaw;
GLfloat Pitch;
// Camera options
GLfloat MovementSpeed;
GLfloat MouseSensitivity;
GLfloat Zoom;

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : front_vector(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY)
{
	this->position = position;
	this->world_up_vector = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateCameraVectors();
}
// Constructor with scalar values
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front_vector(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY)
{
	this->position = glm::vec3(posX, posY, posZ);
	this->world_up_vector = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(this->position, this->position + this->front_vector, this->up_vector);
}

glm::mat4 Camera::GetCameraMatrix(GLfloat width, GLfloat height)
{
	return glm::perspective(glm::radians(FOV), width / height, NearZ, FarZ);
}

glm::vec3 Camera::GetDirection()
{
	return front_vector;
}


// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::HandleKeyboard(CameraMovement direction, GLfloat deltaTime)
{
	GLfloat velocity = this->MovementSpeed * deltaTime;
	if (direction == FORWARD)
		this->position += this->front_vector * velocity;
	if (direction == BACKWARD)
		this->position -= this->front_vector * velocity;
	if (direction == LEFT)
		this->position -= this->right_vector * velocity;
	if (direction == RIGHT)
		this->position += this->right_vector * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::HandleMouseMove(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	if (initial_move) {
		xoffset = 0;
		yoffset = 0;
		initial_move = false;
	}
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->yaw += xoffset;
	this->pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (this->pitch > 89.0f)
			this->pitch = 89.0f;
		if (this->pitch < -89.0f)
			this->pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	this->updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
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
std::shared_ptr<Camera> Camera::GetCamera()
{
	return std::make_shared<Camera>(*this);
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
