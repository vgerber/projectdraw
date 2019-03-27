#pragma once
#include "light.h"

class PointLight : public Light {
public:
	
	GLfloat attenuationConstant = 1.0f;
	GLfloat attenuationLinear = 0.7f;
	GLfloat attenuationQuadratic = 0.007f;

	PointLight();

	///bind opengl resources for shadow mapping
	void beginShadowMapping();

	///unbind opengl resources and complete mapping
	void endShadowMapping();

	///get depth map from shadow mapping (as cubemap)
	DepthMap getCubeDepthMap();

	///set position of light (recalcs cubemap matrices)
	void translate(glm::vec3 translation) override;

	///set maximum light distance
	void setDistance(float distance);

	///get maximum light distance
	float getDistance();

	///free opengl resources
	void dispose() override;
private:
	DepthMap depthMap;
	const GLuint SHADOW_C_WIDTH = 1024, SHADOW_C_HEIGHT = 1024;
	GLfloat aspect = (GLfloat)SHADOW_C_WIDTH / (GLfloat)SHADOW_C_HEIGHT;
	GLfloat nearPlane = 0.0f;
	GLfloat distance = 6.0f;
	glm::mat4 shadowProj;

	std::vector<glm::mat4> shadowTransforms;
	void setup();
	void setupShadowCube();
};