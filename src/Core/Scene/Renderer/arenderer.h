#pragma once
#include "GL/glew.h"

class AbstractRenderer {
public:
    AbstractRenderer(int width, int height);
    
    virtual void clearScreen() = 0;

    virtual GLuint getScreenTexture() = 0;
    virtual void resize(int width, int height);

    int getWidth();
    int getHeight();

    int getRendererId();
private:
    static int globalRendererId;
    int rendererId;
    int width = 0, height = 0;
};