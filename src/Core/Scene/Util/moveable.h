#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "../../Model/box.h"

/*
http://www.opengl-tutorial.org/cn/intermediate-tutorials/tutorial-17-quaternions/

*/

class Moveable : public BoundingBox {
public:

	glm::mat4 getModelMatrix();

	virtual void setCenter(glm::vec3 center);
	virtual void setPosition(glm::vec3 position);
	virtual void rotate(float x, float y, float z);
	virtual void rotate(glm::vec3 vrotation);
	virtual void translate(float x, float y, float z);
	virtual void translate(glm::vec3 vtranslation);
	virtual void scale(float x, float y, float z);
	virtual void scale(glm::vec3 vscale);
	
	virtual void setCenterInWorld(glm::vec3 point);

	Size getSize()					   override;
	void scaleToSize(Size size)		   override;
	void scaleToWidth(float width)	   override;
	void scaleToHeight(float height)   override;
	void scaleToLength(float depth)    override;



	glm::vec3 getCenter();
	glm::vec3 getCenterPoint();
	glm::vec3 getScale();
	glm::vec3 getRotation();
	glm::vec3 getPosition();
	glm::vec3 getPositionCenter();
	
protected:
	Size size;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 vrotation = glm::vec3(0.0f);
	glm::vec3 vscale = glm::vec3(1.0f);
	glm::vec3 vcenter = glm::vec3(0.5f);
	glm::mat4 mmodel = glm::mat4(1.0f);


	
	virtual void updateModel();
};