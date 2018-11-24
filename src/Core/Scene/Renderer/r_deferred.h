#pragma once


#include "../../core.h"
#include "../Camera/camera.h"
#include "../Camera/scenecamera.h"
#include "../drawable.h"
#include "../Light/lights.h"
#include "../Particle/particlegenerator.h"
#include "../Instancing/instancer.h"
#include "../Animation/animatable.h"
#include "../Text/text.h"

#include "arenderer.h"


class DeferredRenderer : public AbstractRenderer {
public:	

    DeferredRenderer(int width, int height, Camera &camera);   



    virtual void resize(int width, int height) override;

    virtual void clearScreen() override;

	virtual void render();

	virtual void addSceneObject(SceneObject &sceneObject);
	virtual void removeSceneObject(SceneObject &sceneObject);

	virtual GLuint getTexture() override;

	virtual void dispose() override;

	virtual void refreshShaderRenderer();

	virtual int getRendererType() override;

protected:
	static const int RendererType = 1;
	bool invalidShaders = false;

	//light shaders
	Shader shaderDLight;
	Shader shaderDLightShadow;
	Shader shaderPLight;
	Shader shaderPLightShadow;
	Shader shaderSLight; 
	Shader shaderSLightShadow; 
	Shader shaderTexture;
	
	//object drawing shader
	Shader shaderBasic;
	Shader shaderInstancing;
	Shader shaderFont;
	Shader shaderLight;
	

	//util shader
	Shader shaderNormals;

	//post
	Shader shaderFXAA;
	Shader shaderHDR;
	Shader shaderBloomBlur;
	Shader shaderBloomMerge;


	GLuint gBufferFBO = 0;
	GLuint rboGDepth = 0;
	GLuint gBufferPosition = 0;
    GLuint gBufferNormal = 0;
    GLuint gBufferAlbedo = 0;
    GLuint gBufferOption1 = 0;
    GLuint gBufferGlow = 0;
    GLuint gDepthStencil = 0;

	GLuint bloomFBO = 0;
	

	GLuint uboMatrices = 0;

	GLuint screenRectFBO = 0;
	GLuint screenRectVBO = 0;
	GLuint screenRectVAO = 0;
	GLuint screenRectTexture = 0;

	   
	GLuint tmpRenderTexture = 0;


	static const int bloomSample = 4;
	GLuint bloomTextures[bloomSample];
	GLuint tmpbloomTextures[bloomSample];


	std::vector<Drawable*> objects;

	//lights
	DirectionalLight* directionalLight = nullptr;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

	std::vector<Animatable*> animatables;

	//Instancer
	std::vector<Instancer*> instancers;


	virtual void renderObjects() override;

	virtual void renderLight() override;

	virtual void applyAntialias();
	virtual void applyBloom();
	virtual void applyHDR();

	void setup();

};