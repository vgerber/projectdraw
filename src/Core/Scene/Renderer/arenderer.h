#pragma once
#include "GL/glew.h"

#include "../Camera/camera.h"


class RenderLight {
public:
    static const int DIRECTIONAL = 0;
    static const int POINT = 1;
    static const int SPOT = 2;
};

enum RenderMode {
	POINTR = 0,
	LINER = 1,
	FILLR = 2
};

class AbstractRenderer {
public:
    AbstractRenderer(int width, int height, const Camera &camera);
    

    virtual GLuint getScreenTexture() = 0;
    virtual void resize(int width, int height);

    virtual void clearScreen() = 0;
    virtual void setRenderMode(RenderMode rmode);

    virtual void beginScene() = 0;
    virtual void endScene() = 0;

    virtual void beginObjects() = 0;
    virtual void endObjects() = 0;

    virtual void beginLight(int lightType) = 0;
    virtual void endLight() = 0;

    virtual void render() = 0;

    int getWidth();
    int getHeight();

    int getRendererId();
private:
    static int globalRendererId;
    int rendererId;
    int width = 0, height = 0;
protected:
    RenderMode renderMode;
    const Camera *camera;
    
};