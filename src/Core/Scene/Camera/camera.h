#pragma once

#include <glm/glm.hpp>

#include "Core/Scene/sceneobject.h"

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


class Camera : public SceneObject
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f));
	~Camera();

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 getViewMatrix();	
	glm::mat4 getProjectionMatrix();

	void setSize(float width, float height);
	void setClipping(float near, float far);

	float getWidth();
	float getHeight();

	float getClippingNear();
	float getClippingFar();

	virtual ViewFrustum getViewFrustum(int splits = 1) = 0;
	
private:
	void transformChanged() override;

	void updateDirection(Transform transform) override;

protected:
	float width = 10.0f;
	float height = 10.0f;
	float nearZ = 0.1f;
	float farZ = 700.0f;

	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	//called after transformation/direction change
	void updateViewMatrix();

	///called after matrix parameter change
	virtual void updateProjectionMatrix() = 0;
};

