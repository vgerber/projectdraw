#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <stdio.h>

#include "rotator.h"
#include "../../Model/box.h"

/*
http://www.opengl-tutorial.org/cn/intermediate-tutorials/tutorial-17-quaternions/

*/

class Transform {
public:
	Transform();
	Transform(glm::vec3 translation, Rotator rotator, glm::vec3 scale);

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
	glm::mat4 transfromMatrix = glm::mat4(1.0);

	void updateMatrix();
};

class Moveable : public BoundingBox {
public:

	glm::mat4 getModelMatrix();
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(glm::vec3 position);
	virtual void rotate(Rotator rotator);
	virtual void scale(float x, float y, float z);
	virtual void scale(glm::vec3 scaling);


	Size getSize()					   override;
	void scaleToSize(Size size)		   override;
	void scaleToWidth(float width)	   override;
	void scaleToHeight(float height)   override;
	void scaleToLength(float depth)    override;


	glm::vec3 getScale();	
	glm::vec3 getPosition();
	Rotator getRotator();	
protected:
	Size size;
	Transform transform;

	virtual void transformChanged();
};