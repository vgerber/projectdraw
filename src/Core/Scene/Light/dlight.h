#pragma once

#include "light.h"

class DirectionalLight : public Light {
public:
	glm::vec3 direction;

	void Apply(Shader shader, std::string target);
};