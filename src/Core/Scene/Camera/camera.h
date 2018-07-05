#pragma once
#include "../sceneobject.h"
#include "../../Model/box.h"

#include <memory>

struct ViewFrustum {
	GLfloat nearZ = 0.0f;
	GLfloat farZ = 0.0f;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;


	std::vector<std::vector<glm::vec3>> splits;
};

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const GLfloat YAW = 90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.07f;

class Camera : public SceneObject
{
public:

	float NearZ = 0.1f;
	float FarZ = 700.0f;
	float FOV = 45.0f;

	Camera(glm::vec3 position);
	~Camera();

	void setPosition(glm::vec3 position);

	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);


	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	glm::mat4 GetCameraMatrix(GLfloat width, GLfloat height);

	glm::vec3 GetDirection();

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void HandleKeyboard(CameraMovement direction, GLfloat deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void HandleMouseMove(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void HandleMouseScroll(GLfloat yoffset);

	std::shared_ptr<Camera> GetCamera();

	virtual ViewFrustum getViewFrustum(int splits = 1);

	virtual void dispose();
private:
	bool initial_move = true;
	glm::vec3 front_vector;
	glm::vec3 up_vector;
	glm::vec3 right_vector;
	glm::vec3 world_up_vector;
	// Eular Angles
	GLfloat yaw;
	GLfloat pitch;
	GLfloat roll;

	void updateCameraVectors();
};

