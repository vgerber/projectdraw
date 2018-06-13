#pragma once
#include "../../drawable.h"

#include <vector>

class Cube : public Drawable
{
public:
	Cube(float x, float y, float z, float width, float depth, float height, glm::vec4 color);
	~Cube();
};

