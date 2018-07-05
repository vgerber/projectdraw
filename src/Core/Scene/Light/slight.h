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

	void beginShadowMapping();
	void endShadadowMapping();

	GLuint getShadowMap();

	void setDirection(glm::vec3 direction);
	void setDistance(float distance);

	void apply(Shader shader, std::string target);
	void setCutOff(float inner, float outer);
	void dispose();
private:
	DepthMap depthMap;
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	float distance = 100.0f;

	void setup();
};