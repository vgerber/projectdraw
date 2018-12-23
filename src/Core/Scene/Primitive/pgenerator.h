#pragma once

#include "Core/Mesh/basicmesh.h"
#include "geometry.h"

#include <cmath>
#include <functional>

namespace pd {

	Mesh * generateRectangle(GLfloat width, GLfloat height, glm::vec4 color);

	Mesh * generateCircle(GLfloat radius, GLfloat quality, glm::vec4 color);

	Mesh * generateQuad(GLfloat width, GLfloat height, GLfloat depth, glm::vec4 color);

	Mesh * generateHeightfield(int width, int length, std::vector<float> data);

	Mesh * generateHeightfieldStep(int width, int length, std::vector<float> data, glm::vec4 color);

	Mesh * generateSphere(int lats, int longs, glm::vec4 color);

	Mesh * generateCone(float radius, float height, float quality, glm::vec4 color);

	Mesh * generateCylinder(float radius, float height, float quality, glm::vec4 color);

	Geometry * geometryCircle(float radius, float quality, glm::vec4 color);
}