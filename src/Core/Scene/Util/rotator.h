#pragma once


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include <math.h>
#include <vector>

class Rotator {
public:
    Rotator();
    Rotator(glm::quat rotation, glm::vec3 origin);

	/*
	Set Center of Rotation
	*/
	void setOrigin(float x, float y, float z);
	void setOrigin(glm::vec3 origin);

	/*
	Rotate by euler angles
	*/
    void rotateEuler(float x, float y, float z);
    void rotateEuler(glm::vec3 rotation);

	/*
	Rotate by quanterion
	*/
    void rotate(glm::quat rotation);

	/*
	Rotate around an rotationAxis
	*/
    void rotateAxis(float radians, glm::vec3 rotationAxis);

	/*
	Apply a new rotation to the existing one

	*/
    void applyRotation(glm::quat rotation);
    void applyRotation(glm::vec3 euler);

	/*
	Apply a new rotation matrix
	->includes rotator origin rotation
	*/
    void applyRotation(Rotator rotator);

	/*
	Rotate from vecFrom to vecTo
	*/
    void vectorRotation(glm::vec3 vecFrom, glm::vec3 vecTo);

	/*
	Interpolate rotation 1 and 2 with factor x
	*/
    void interpolate(glm::quat rotation, float factor);
    void interpolate(Rotator rotator, float factor);

	glm::vec3 getOrigin();

    glm::vec3 getRotationAxis();
    glm::vec3 getRotationEuler();
    glm::quat getRotation();
    glm::mat4 getRotationMatrix();

    bool isEqual(Rotator rotator);
    bool isEqual(glm::quat rotation);

	void clearAppliedRotators();
private:
	glm::vec3 origin;
	glm::quat rotation;
	glm::mat4 rotationMatrix;

	std::vector<glm::mat4> appliedMatrices;

	void updateMatrix();
};