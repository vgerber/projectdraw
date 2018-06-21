#pragma once

#include "../core.h"

struct Size {
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f, width = 0.0f, height = 0.0f, depth = 0.0f;
};

class BoundingBox {
	virtual Size getSize() = 0;
	virtual void scaleToSize(Size size) = 0;
	virtual void scaleToWidth(GLfloat width) = 0;
	virtual void scaleToHeight(GLfloat height) = 0;
	virtual void scaleToLength(GLfloat depth) = 0;
};