#pragma once
#include "light.h"

class PointLight : public Light {
public:
	GLfloat radius = 6.0f;
	GLfloat attenuationConstant = 1.0f;
	GLfloat attenuationLinear = 0.7f;
	GLfloat attenuationQuadratic = 0.007f;

	PointLight();
	void apply(Shader shader, std::string target);
	void beginShadowMapping();
	void endShadowMapping();
	GLuint getShadowCubeMap();
	void setPosition(glm::vec3 position);
private:
	GLuint depthCubeMap;
	const GLuint SHADOW_C_WIDTH = 1024, SHADOW_C_HEIGHT = 1024;
	GLuint depthCubemapFBO;
	GLfloat aspect = (GLfloat)SHADOW_C_WIDTH / (GLfloat)SHADOW_C_HEIGHT;
	GLfloat nearPlane = 1.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, nearPlane, radius);

	std::vector<glm::mat4> shadowTransforms;
	void setup();
	void setupShadowCube();
};