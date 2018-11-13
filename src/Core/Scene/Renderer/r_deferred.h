#pragma once


#include "../../core.h"
#include "../Camera/camera.h"
#include "../Camera/scenecamera.h"
#include "../drawable.h"
#include "../Light/lights.h"
#include "../Particle/particlegenerator.h"
#include "../Instancing/instancer.h"
#include "../Animation/animatable.h"

#include "arenderer.h"

struct DeferredRendererConfig
{
	bool visibleDirectionalLight = true;
	bool visiblePointLight = false;
	bool visibleSpotLight = false;
};

class DeferredRenderer : public AbstractRenderer {
public:
	DeferredRendererConfig dConfig;

    DeferredRenderer(int width, int height, Camera &camera);    



    virtual void resize(int width, int height) override;

    virtual void clearScreen() override;

	virtual void render();

	virtual void addSceneObject(SceneObject &sceneObject);
	virtual void removeSceneObject(SceneObject &sceneObject);

	virtual GLuint getTexture() override;

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
	

	GLuint uboMatrices = 0;

	GLuint screenRectFBO = 0;
	GLuint screenRectVBO = 0;
	GLuint screenRectVAO = 0;
	GLuint screenRectTexture = 0;



	std::vector<Drawable*> objects;

	//lights
	DirectionalLight* directionalLight = nullptr;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

	std::vector<Animatable*> animatables;

	//Instancer
	std::vector<Instancer*> instancers;


	virtual void renderObjects() override;
	virtual void bloom();

	virtual void renderLight() override;

	void setup();

};