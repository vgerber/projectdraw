#pragma once

#include "Core/Shader/shader.h"
#include "Core/Resource/resourcemanager.h"
#include "Core/Texture/Filter/filter.h"
#include "Core/Util/Debug/debug.hpp"

class HDR : public Filter {
public:
    virtual void apply(unsigned int texture);

    virtual void resize(int width, int height) override;
	virtual void clear() override;
	virtual void dispose() override;
	virtual void setup() override;
private:
    Shader shaderHdr;

    int width;
    int height;

    unsigned int hdrFBO;
    unsigned int hdrTexture;

    unsigned int rectVAO;
    unsigned int rectVBO;
};