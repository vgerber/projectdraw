#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <stdio.h>

#include "rotator.h"
#include "Core/Mesh/Util/box.h"

class Transform {
public:
	Transform();
	Transform(glm::vec3 translation, Rotator rotator, glm::vec3 scale);
	Transform(glm::mat4 transform);

	void translate(glm::vec3 translation);
	void rotate(Rotator rotator);
	void scale(glm::vec3 scaling);

	glm::vec3 getTranslation();
	glm::vec3 getScale();
	Rotator getRotation();

	glm::mat4 getInverse();
	glm::mat4 getMatrix();

	Transform operator* (const Transform &transform);

	void print();
private:
	glm::vec3 translation = glm::vec3(0.0);
	glm::vec3 scaling = glm::vec3(1.0);
	Rotator rotator;
	glm::mat4 transformMatrix = glm::mat4(1.0);

	void updateMatrix();
};

class Moveable : public BoundingBox {
public:

	glm::mat4 getModelMatrix();
	Transform getTransform();

	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(glm::vec3 position);
	virtual void rotate(Rotator rotator);
	virtual void scale(float x, float y, float z);
	virtual void scale(glm::vec3 scaling);
	virtual void setTransform(Transform transform);

	void setForward(glm::vec3 forwardDirection);

	void setUp(glm::vec3 upDirection);

	void setForwardUp(glm::vec3 forwardDircetion, glm::vec3 upDirection);

	/*
	* Flips right vector (* -1)
	*/
	void flipRight();

	///returns transformed forward vector
	glm::vec3 getForward();
	///returns transformed up vector
	glm::vec3 getUp();
	///returns transformed right vector
	glm::vec3 getRight();

	glm::vec3 getBaseForward();
	glm::vec3 getBaseUp();
	glm::vec3 getBaseRight();

	Size getSize()					 override;
	void scaleToSize(Size size)		 override;
	void scaleToWidth(float width)	 override;
	void scaleToHeight(float height) override;
	void scaleToDepth(float depth)  override;


	glm::vec3 getScale();	
	glm::vec3 getPosition();
	Rotator getRotator();	
protected:
	Size size;
	Transform transform;

	virtual void transformChanged();
	virtual void updateDirection(Transform transform);

private:
	glm::vec3 forward = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 up      = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 right   = glm::vec3(0.0, -1.0, 0.0);

	///Cached direction vectors with model transforms
	glm::vec3 transForward = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 transUp      = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 transRight   = glm::vec3(0.0, -1.0, 0.0);

};