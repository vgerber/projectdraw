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
	/**
	 * @brief Construct a new Camera object
	 * 
	 * @param position initial camera position
	 * @param forward look direction
	 * @param up camera up direction
	 */
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f));
	
	/**
	 * @brief Destroy the Camera object
	 * 
	 */
	~Camera();

	/**
	 * @brief Get the View Matrix object
	 * 
	 * @return glm::mat4 
	 */
	glm::mat4 getViewMatrix();	

	/**
	 * @brief Get the Projection Matrix object
	 * 
	 * @return glm::mat4 
	 */
	glm::mat4 getProjectionMatrix();

	/**
	 * @brief Set the Size
	 * 
	 * the size will be used for aspect ratio, camera width/height
	 * 
	 * @param width scene width
	 * @param height scene height
	 */
	void setSize(float width, float height);

	/**
	 * @brief Set the Clipping object
	 * 
	 * @param near near clipping
	 * @param far far clipping
	 */
	void setClipping(float near, float far);

	/**
	 * @brief Get the Width
	 * 
	 * @return float 
	 */
	float getWidth();

	/**
	 * @brief Get the Height
	 * 
	 * @return float 
	 */
	float getHeight();

	/**
	 * @brief Get the Clipping Near
	 * 
	 * @return float 
	 */
	float getClippingNear();

	/**
	 * @brief Get the Clipping Far
	 * 
	 * @return float 
	 */
	float getClippingFar();

	/**
	 * @brief Get the ViewFrustum
	 * 
	 * @param splits split viefrustum in subsections
	 * @return ViewFrustum 
	 */
	virtual ViewFrustum getViewFrustum(int splits = 1) = 0;
	
private:
	/**
	 * @brief called when transform changed
	 * 
	 */
	void transformChanged() override;

	/**
	 * @brief called when transform changed
	 * 
	 * @param transform 
	 */
	void updateDirection(Transform transform) override;

protected:
	float width = 10.0f;
	float height = 10.0f;
	float nearZ = 0.1f;
	float farZ = 700.0f;

	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	/**
	 * @brief update matrix when transform changed
	 * 
	 */
	void updateViewMatrix();

	/**
	 * @brief update matrix when transform changed
	 * 
	 */
	virtual void updateProjectionMatrix() = 0;
};

