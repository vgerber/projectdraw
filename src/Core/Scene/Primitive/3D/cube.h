#pragma once
#include "../../drawable.h"

#include <vector>

class Cube : public Drawable
{
public:
	Cube(float x, float y, float z, float width, float length, float height, glm::vec4 color);
	~Cube();
};

