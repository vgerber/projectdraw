#pragma once

#include "Core/Shader/shader.h"
#include "Core/Resource/resourcemanager.h"
#include "Core/Texture/Filter/filter.h"
#include "Shaders/Postprocessing/Antialias/SMAA/AreaTex.h"
#include "Shaders/Postprocessing/Antialias/SMAA/SearchTex.h"
#include "Core/Util/Debug/debug.hpp"

class SMAA : public Filter {
public:
	virtual void apply(unsigned int texture);

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