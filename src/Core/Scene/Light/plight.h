#pragma once
#include "light.h"

class PointLight : public Light {
public:
	GLfloat constant = 1.0f;
	GLfloat linear = 0.0f;
	GLfloat quadratic = 0.0f;

	void apply(Shader shader, std::string target);
	void begin_shadow_mapping();
	void end_shadow_mapping();
	GLuint get_shadow_cube_map();
	void SetPosition(glm::vec3 position);
private:
	GLuint depthCubeMap;
	const GLuint SHADOW_C_WIDTH = 1024, SHADOW_C_HEIGHT = 1024;
	GLuint depthCubemapFBO;
	GLfloat aspect = (GLfloat)SHADOW_C_WIDTH / (GLfloat)SHADOW_C_HEIGHT;
	GLfloat near = 1.0f;
	GLfloat far = 100.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	std::vector<glm::mat4> shadowTransforms;
	void setup();
	void setup_shadow_cube();
};