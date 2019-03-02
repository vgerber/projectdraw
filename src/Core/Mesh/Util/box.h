#pragma once


struct Size {
	Size(float width = 0.0f, float height = 0.0f, float depth = 0.0f);
	Size(float offsetX, float offsetY, float offsetZ, float width, float height, float depth);

	//offset x
	float x = 0.0f;

	//offset y
	float y = 0.0f;

	//offset z
	float z = 0.0f;


	float width = 0.0f;
	float height = 0.0f;
	float depth = 0.0f;
};

class BoundingBox {
	virtual Size getSize() = 0;
	virtual void scaleToSize(Size size) = 0;
	virtual void scaleToWidth(float width) = 0;
	virtual void scaleToHeight(float height) = 0;
	virtual void scaleToDepth(float depth) = 0;
};