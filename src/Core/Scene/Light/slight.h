#pragma once

#include "light.h"

class SpotLight : public Light {
public:
	float attenuationLinear = 0.0f;
	float attenuationConstant = 1.0f;
	float attenuationQuadratic = 0.0f;

	float cutOff = 0.0f;
	float outerCutOff = 0.0f;
	glm::vec3 direction;

	SpotLight();

	///bind opengl resources for shadow mapping
	void beginShadowMapping();

	///unbind opengl resources and complete shadow mapping
	void endShadowMapping();

	///set ray direction
	void setDirection(glm::vec3 direction);

	///get ray direction
	glm::vec3 getDirection();

	///set maximum light distance
	void setDistance(float distance);
	
	///get maximum light distance
	float getDistance();

	///get depth map from shadow mapping
	DepthMap getDepthMap();

	///set light cutoff
	void setCutOff(float inner, float outer);

	///free opengl resources
	virtual void dispose() override;
private:
	DepthMap depthMap;
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	float distance = 200.0f;

	void setup();
};