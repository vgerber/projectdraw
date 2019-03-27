#pragma once

#include "Core/Shader/shader.h"
#include "Core/Resource/resourcemanager.h"
#include "Core/Texture/Filter/filter.h"
#include "Core/Util/Debug/debug.hpp"

class HDR : public Filter {
public:
    /**
     * @brief apply filter on texture
     * 
     * @param texture target fro hdr
     */
    virtual void apply(unsigned int texture);

    /**
     * @brief resize filter to target texture size
     * 
     * @param width texture width
     * @param height texture height
     */
    virtual void resize(int width, int height) override;

    /**
     * @brief clear filter buffers
     * 
     */
	virtual void clear() override;

    /**
     * @brief free filter resources
     * 
     */
	virtual void dispose() override;

    /**
     * @brief initialize filter resources
     * 
     */
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