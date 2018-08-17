#pragma once

#include "light.h"
#include "../Camera/camera.h"


class DirectionalLight : public Light {
public:
	glm::vec3 minVec;
	glm::vec3 maxVec;

	DirectionalLight();
	void change_direction(glm::vec3 direction);
	glm::vec3 get_direction();
	void apply(Shader shader, std::string target);

	void setViewFrustum(ViewFrustum viewFrustum);

	void begin_shadow_mapping(int slice = 1);
	void end_shadow_mapping();

	GLuint getShadowMap(int slice = 0);
	const int getCSMSlices();

	void dispose();
private:
	const int csmSlices = 3;
	std::vector<DepthMap> depthMaps;
	GLfloat distance = 0.0f;

	glm::vec3 direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	const GLuint SHADOW_WIDTH = 1028, SHADOW_HEIGHT = 1028;

	void setup();
};