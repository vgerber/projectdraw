#pragma once

#include "Core/Mesh/Primitives/mesh.h"

#include <cmath>
#include <functional>

namespace pd {

	Mesh rectangle(GLfloat width, GLfloat height, glm::vec4 color);

	Mesh circle(GLfloat radius, GLfloat quality, glm::vec4 color);

	Mesh quad(GLfloat width, GLfloat height, GLfloat depth, glm::vec4 color);

	Mesh heightfield(int width, int length, std::vector<float> data);

	Mesh heightfieldStep(int width, int length, std::vector<float> data, glm::vec4 color);

	Mesh sphere(int lats, int longs, glm::vec4 color);

	Mesh cone(float radius, float height, float quality, glm::vec4 color);

	Mesh cylinder(float radius, float height, float quality, glm::vec4 color);
	
	Mesh circleLine(float radius, float quality, glm::vec4 color);
}