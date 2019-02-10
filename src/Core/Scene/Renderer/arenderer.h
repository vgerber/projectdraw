#pragma once
#include "GL/glew.h"

#include "../Camera/camera.h"


enum RenderMode {
	POINTR = 0,
	LINER = 1,
	FILLR = 2
};

class AbstractRenderer {
public:
    AbstractRenderer(int width, int height, Camera &camera);
    
    virtual void resize(int width, int height);

    virtual void clearScreen() = 0;
    virtual void setRenderMode(RenderMode rmode);


    virtual void render() = 0;

	virtual void addSceneObject(SceneObject &sceneObject) = 0;
	virtual void removeSceneObject(SceneObject &sceneObject) = 0;

    virtual GLuint getTexture() = 0;

    virtual void dispose() = 0;

    int getWidth();
    int getHeight();

    int getRendererId();

	void invalidateShader();
private:
    static int globalRendererId;
    int rendererId;
    int width = 0, height = 0;
protected:
    RenderMode renderMode;
    Camera *camera;
	bool invalidShaders = false;    
};