#pragma once

#include "../core.h"

struct Size {
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f, width = 0.0f, height = 0.0f, depth = 0.0f;
};

class BoundingBox {
	virtual Size get_size() = 0;
	virtual void scale_to_size(Size size) = 0;
	virtual void scale_to_width(GLfloat width) = 0;
	virtual void scale_to_height(GLfloat height) = 0;
	virtual void scale_to_length(GLfloat depth) = 0;
};