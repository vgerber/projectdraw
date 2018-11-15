#pragma once

#include <glm/glm.hpp>

#include "../sceneobject.h"
#include "../../Model/box.h"

#include <memory>
#include <cmath>
#include <vector>


struct ViewFrustum {
	float nearZ = 0.0f;
	float farZ = 0.0f;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;


	std::vector<std::vector<glm::vec3>> splits;
};


// Default camera values
const float YAW = 90.0f;
const float PITCH = 0.0f;
const float SPEED = 3.0f;
const float SENSITIVTY = 0.07f;

class Camera : public SceneObject
{
public:
	float Width = 10.0f;
	float Height = 10.0f;
	float NearZ = 0.1f;
	float FarZ = 700.0f;

	Camera(glm::vec3 position);
	~Camera();

	void setPosition(glm::vec3 position) override;


	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);


	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 getViewMatrix();	

	void setFront(glm::vec3 frontV);
	void setUp(glm::vec3 upV);

	glm::vec3 getFront();
	glm::vec3 getRight();
	glm::vec3 getUp();

	void lookAt(glm::vec3 target);

	void updateCameraVectors();

	virtual ViewFrustum getViewFrustum(int splits = 1) = 0;
	virtual glm::mat4 getCameraMatrix() = 0;
	virtual void dispose();
private:
	bool initial_move = true;	

protected:
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	glm::vec3 front_vector;
	glm::vec3 up_vector;
	glm::vec3 right_vector;
	glm::vec3 world_up_vector;
};

