#pragma once

#include "../../Model/model.h"

#include <cmath>

namespace primitives {

	Model generate_rectangle(GLfloat width, GLfloat height, glm::vec4 color);

	Model generate_circle(GLfloat radius, GLfloat quality, glm::vec4 color);

	Model generate_quad(GLfloat width, GLfloat height, GLfloat depth, glm::vec4 color);

	Model generate_hightfield(int width, int length, std::vector<float> data);

	Model generateHeightfieldStep(int width, int length, std::vector<float> data);
}