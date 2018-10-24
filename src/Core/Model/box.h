#pragma once


struct Size {
	float x = 0.0f, y = 0.0f, z = 0.0f, width = 0.0f, height = 0.0f, depth = 0.0f;
};

class BoundingBox {
	virtual Size getSize() = 0;
	virtual void scaleToSize(Size size) = 0;
	virtual void scaleToWidth(float width) = 0;
	virtual void scaleToHeight(float height) = 0;
	virtual void scaleToLength(float depth) = 0;
};