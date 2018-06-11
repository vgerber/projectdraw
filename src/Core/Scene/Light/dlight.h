#pragma once

#include "light.h"

class DirectionalLight : public Light {
public:
	glm::vec3 direction;

	DirectionalLight();

	void apply(Shader shader, std::string target);

	void begin_shadow_mapping();
	void end_shadow_mapping();

	GLuint get_shadow_map();
private:
	GLuint depthMapFBO;
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMap;
	GLfloat near_plane = 0.1f, far_plane = 100.0f;
	glm::mat4 lightProjection;
	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrix;
};