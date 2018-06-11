#pragma once

#include "../sceneobject.h"
#include "../../Shader/shader.h"


class Light : public SceneObject
{
public:
	GLboolean is_on = true;
	glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

	void apply(Shader shader, std::string target);

	void on();
	void off();
};