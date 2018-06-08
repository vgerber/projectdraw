#pragma once
#include "light.h"

class PointLight : public Light {
public:
	GLfloat constant = 1.0f;
	GLfloat linear = 0.0f;
	GLfloat quadratic = 0.0f;

	void Apply(Shader shader, std::string target);

};