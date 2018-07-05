#pragma once

#include "../drawable.h"
#include "../../Shader/shader.h"

class DepthMap {
public:
	glm::mat4 lightSpaceMatrix = glm::mat4(0.0);
	GLuint depthMap;
	GLuint depthMapFBO;

	void dispose();
};

class Light : public Drawable
{
public:
	bool draw_shadow = false;
	GLfloat intensity = 1.0f;
	glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

	Light();

	void updateModel();

	void apply(Shader shader, std::string target);
};