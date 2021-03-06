#include "camera.h"


// Constructor with scalar values
Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up)
{
	translate(position);
	setForwardUp(forward, up);
	this->updateViewMatrix();
}

Camera::~Camera()
{
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix() const {
	return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
	return projMatrix;
}

void Camera::setSize(float width, float height) {
	this->width = width;
	this->height = height;
	updateProjectionMatrix();
}

void Camera::setClipping(float near, float far) {
	this->nearZ = near;
	this->farZ = far;
	updateProjectionMatrix();
}

float Camera::getWidth() {
	return width;
}

float Camera::getHeight() {
	return height;
}

float Camera::getClippingNear() {
	return nearZ;
}

float Camera::getClippingFar() {
	return farZ;
}

void Camera::updateViewMatrix()
{
	// Also re-calculate the Right and Up vector
	glm::vec3 pos = getWorldTransform().getTranslation();
	//transform direction vecotrs to world
	glm::mat4 worldRotation = getWorldTransform().getRotation().getRotationMatrix();
	glm::vec3 worldForward = worldRotation * glm::vec4(getBaseForward(), 1.0);
	glm::vec3 worldUp = worldRotation * glm::vec4(getBaseUp(), 1.0);
	if(!isRightFlipped()) {
		viewMatrix = glm::lookAtRH(pos, pos + worldForward, worldUp);
	}
	else {
		viewMatrix = glm::lookAtLH(pos, pos + worldForward, worldUp);
	}
	
}

void Camera::transformChanged() {
	SceneObject::transformChanged();
	updateViewMatrix();
}

void Camera::updateDirection(Transform transform) {
	SceneObject::updateDirection(transform);
	updateViewMatrix();
}
