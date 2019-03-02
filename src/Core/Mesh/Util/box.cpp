#include "box.h"

Size::Size(float width, float height, float depth)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
}

Size::Size(float offsetX, float offsetY, float offsetZ, float width, float height, float depth) : Size(width, height, depth)
{
	this->x = offsetX;
	this->y = offsetY;
	this->z = offsetZ;
}
