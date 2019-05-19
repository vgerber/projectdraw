#pragma once
#include "Core/Texture/texture.h"
#include "Core/Renderer/arenderer.h"

class Filter {	
public:
	virtual void resize(int width, int height) = 0;
	virtual void clear() = 0;
	virtual void dispose() = 0;
	virtual void setup() = 0;

protected:
};

