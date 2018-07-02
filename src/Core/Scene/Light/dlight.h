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

	void begin_shadow_mapping();
	void end_shadow_mapping();

	GLuint get_shadow_map();
private:
	glm::vec3 direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	GLuint depthMapFBO;
	const GLuint SHADOW_WIDTH = 1000, SHADOW_HEIGHT = 1000;
	GLuint depthMap;
	glm::mat4 lightProjection;
	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrix;

	void setup();
};