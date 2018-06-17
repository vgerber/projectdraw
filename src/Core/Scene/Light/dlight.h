#pragma once

#include "light.h"

class DirectionalLight : public Light {
public:
	

	DirectionalLight();
	void change_direction(glm::vec3 direction);
	glm::vec3 get_direction();
	void apply(Shader shader, std::string target);

	void begin_shadow_mapping();
	void end_shadow_mapping();

	GLuint get_shadow_map();
private:
	glm::vec3 direction = glm::vec3(-1.0f);
	GLuint depthMapFBO;
	const GLuint SHADOW_WIDTH = 5000, SHADOW_HEIGHT = 5000;
	GLuint depthMap;
	GLfloat near_plane = 0.1f, far_plane = 100.0f;
	glm::mat4 lightProjection;
	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrix;

	void setup();
};