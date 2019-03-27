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

/**
 * @brief Transform utility class
 * 
 */
class Transform {
public:
	/**
	 * @brief Construct a new Transform object
	 * 
	 */
	Transform();

	/**
	 * @brief Construct a new Transform object
	 * 
	 * @param translation 
	 * @param rotator 
	 * @param scale 
	 */
	Transform(glm::vec3 translation, Rotator rotator, glm::vec3 scale);

	/**
	 * @brief Construct a new Transform object from transform matrix
	 * 
	 * @param transform 
	 */
	Transform(glm::mat4 transform);

	/**
	 * @brief Apply translation
	 * 
	 * Replaces last translation
	 * 
	 * @param translation 
	 */
	void translate(glm::vec3 translation);

	/**
	 * @brief Apply rotation
	 * 
	 * Replaces last rotation
	 * 
	 * @param rotator 
	 */
	void rotate(Rotator rotator);

	/**
	 * @brief Apply scaling
	 * 
	 * Replaces last scaling
	 * 
	 * @param scaling 
	 */
	void scale(glm::vec3 scaling);

	/**
	 * @brief Get the transformed position
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 getPosition();

	/**
	 * @brief Get the translation
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 getTranslation();

	/**
	 * @brief Get the scale
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 getScale();

	/**
	 * @brief Get the rotation
	 * 
	 * @return Rotator 
	 */
	Rotator getRotation();

	/**
	 * @brief Get the Inverse object
	 * 
	 * @return glm::mat4 
	 */
	glm::mat4 getInverse();

	/**
	 * @brief Get the Matrix object
	 * 
	 * @return glm::mat4 
	 */
	glm::mat4 getMatrix();

	Transform operator* (const Transform &transform);

	/**
	 * @brief Print transform (debug purpose)
	 * 
	 */
	void print();
private:
	glm::vec3 position = glm::vec3(0.0); //transformed position
	glm::vec3 translation = glm::vec3(0.0);
	glm::vec3 scaling = glm::vec3(1.0);
	Rotator rotator;
	glm::mat4 transformMatrix = glm::mat4(1.0);

	/**
	 * @brief Recalculate transformation matrix
	 * 
	 */
	void updateMatrix();
};

class Moveable : public IBoundingBox {
public:

	/**
	 * @brief Get the Transform object
	 * 
	 * @return Transform 
	 */
	Transform getTransform();

	/**
	 * @brief See @ref Transform#translate
	 * 
	 * @param x 
	 * @param y 
	 * @param z 
	 */
	virtual void translate(float x, float y, float z);

	/**
	 * @brief See q@ref translate(float, float, float)
	 * 
	 * @param translation
	 */
	virtual void translate(glm::vec3 translation);

	/**
	 * @brief See @ref Transform#rotate
	 * 
	 * @param rotator 
	 */
	virtual void rotate(Rotator rotator);

	/**
	 * @brief See @ref Transform#scale
	 * 
	 * @param x 
	 * @param y 
	 * @param z 
	 */
	virtual void scale(float x, float y, float z);

	/**
	 * @brief See @ref scale(float, float, float)
	 * 
	 * @param scaling 
	 */
	virtual void scale(glm::vec3 scaling);

	/**
	 * @brief Override transform with new transform
	 * 
	 * @param transform 
	 */
	virtual void setTransform(Transform transform);

	/**
	 * @brief Set the forward direction
	 * 
	 * @param forwardDirection 
	 */
	void setForward(glm::vec3 forwardDirection);

	/**
	 * @brief Set the up direction
	 *  
	 * @param upDirection 
	 */
	void setUp(glm::vec3 upDirection);

	/**
	 * @brief Set the forward and up direction
	 * 
	 * Use this function as direction intialisation
	 * 
	 * @param forwardDircetion 
	 * @param upDirection 
	 */
	void setForwardUp(glm::vec3 forwardDircetion, glm::vec3 upDirection);

	/**
	 * @brief flip right direction
	 * 
	 * Right direction can no be set directly.
	 * Use this function if the calculated right direction from up and forward is in the wrong direction.
	 * 
	 */
	void flipRight();

	/**
	 * @brief Get the transformed forward vector
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 getForward();
	
	/**
	 * @brief Get the transformed up vector
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 getUp();
	
	/**
	 * @brief Get the transformed right vector
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 getRight();

	/**
	 * @brief Get the untransformed forward vector
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 getBaseForward();

	/**
	 * @brief Get the untransformed up vector
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 getBaseUp();

	/**
	 * @brief Get the untransformed right vector
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 getBaseRight();

	/**
	 * @brief Get the Size
	 * 
	 * @return Size 
	 */
	Size getSize()					 override;

	/**
	 * @brief set scale to matchin bounding size
	 * 
	 * @param size 
	 */
	void scaleToSize(Size size)		 override;

	/**
	 * @brief See @ref scaleToSize
	 * 
	 * @param width 
	 */
	void scaleToWidth(float width)	 override;

	/**
	 * @brief See @ref scaleToSize
	 * 
	 * @param height 
	 */
	void scaleToHeight(float height) override;

	/**
	 * @brief See @ref scaleToSize
	 * 
	 * @param depth 
	 */
	void scaleToDepth(float depth)  override;

	/**
	 * @brief Get the Translation object
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 getTranslation();

	/**
	 * @brief Get the Scale object
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 getScale();

	/**
	 * @brief Get the Position object
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 getPosition();

	/**
	 * @brief Get the Rotator object
	 * 
	 * @return Rotator 
	 */
	Rotator getRotator();	
protected:
	Size size;
	Transform transform;

	/**
	 * @brief called when transformation property is changed
	 * 
	 */
	virtual void transformChanged();

	/**
	 * @brief transform direction vectors
	 * 
	 * @param transform new transform
	 */
	virtual void updateDirection(Transform transform);

	/**
	 * @brief true if right vector is flipped 
	 * 
	 * @return true 
	 * @return false 
	 */
	bool isRightFlipped();

private:
	glm::vec3 forward = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 up      = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 right   = glm::vec3(0.0, -1.0, 0.0);

	bool rightFlipped = false; //opposite value will be set if @ref flipRight is called

	///Cached direction vectors with model transforms
	glm::vec3 transForward = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 transUp      = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 transRight   = glm::vec3(0.0, -1.0, 0.0);

};