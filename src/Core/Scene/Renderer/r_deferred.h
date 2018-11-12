#pragma once

#include "arenderer.h"
#include "../../Shader/shader.h"

class DeferredRenderer : public AbstractRenderer {
public:
    DeferredRenderer(int width, int height, const Camera &camera);    

    virtual void resize(int width, int height) override;

    virtual void clearScreen() override;

    virtual void beginScene() override;
    virtual void endScene() override;

    virtual void beginObjects() override;
    virtual void endObjects() override;

    virtual void beginLight(int lightType) override;
    virtual void endLight() override;

protected:

    Shader shader_basic;
	Shader shader_light;
	Shader shader_normals;
	Shader shader_geometry;



	GLuint gBufferFBO = 0;
	GLuint rboGDepth = 0;
	GLuint gBufferPosition = 0;
    GLuint gBufferNormal = 0;
    GLuint gBufferAlbedo = 0;
    GLuint gBufferOption1 = 0;
    GLuint gBloom = 0;
    GLuint gDepthStencil = 0;

	GLuint bloomFBO = 0;
	GLuint bloomTexture = 0;

	GLuint plightFBO = 0;
	GLuint plightTexture = 0;

	GLuint lightFBO = 0;
	GLuint lightTexture = 0;

	GLuint uboMatrices = 0;
	GLuint screenRectVBO = 0;
	GLuint screenRectVAO = 0;

	GLuint screenShadowVBO = 0;
	GLuint screenShadowVAO = 0;

    void setup();



};