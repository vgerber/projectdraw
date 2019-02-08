#pragma once

#include "Core/Shader/shader.h"
#include "Core/Texture/Filter/filter.h"
#include "Shaders/Experimental/OpenglTest/SMAA/AreaTex.h"
#include "Shaders/Experimental/OpenglTest/SMAA/SearchTex.h"

class SMAA : public Filter {
public:
	virtual void apply(const Texture &texture);

	virtual void resize(int width, int height);

	virtual void clear();

	virtual void dispose();

	virtual void setup();
protected:
	int width, height;

	Shader shaderSMAAEdge;
	Shader shaderSMAAWeight;
	Shader shaderSMAABlend;

	unsigned int smaaFBO;

	unsigned int smaaTexture;
	unsigned int edgeTexture;
	unsigned int weightTexture;
	unsigned int areaTexture;
	unsigned int searchTexture;

	unsigned int screenRectVAO;
	unsigned int screenRectVBO;
};