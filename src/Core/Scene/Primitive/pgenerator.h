#pragma once

#include "../../Model/model.h"
#include "geometry.h"

#include <cmath>
#include <functional>

namespace primitives {

	Model generateRectangle(GLfloat width, GLfloat height, glm::vec4 color);

	Model generateCircle(GLfloat radius, GLfloat quality, glm::vec4 color);

	Model generateQuad(GLfloat width, GLfloat height, GLfloat depth, glm::vec4 color);

	Model generate_hightfield(int width, int length, std::vector<float> data);

	Model generateHeightfieldStep(int width, int length, std::vector<float> data, glm::vec4 color);

	Model generateSphere(int lats, int longs, glm::vec4 color);

	Model generateCone(float radius, float height, float quality, glm::vec4 color);

	Model generateCylinder(float radius, float height, float quality, glm::vec4 color);

	std::vector<Point> geometryCircle(float radius, float quality, glm::vec4 color);
}