#pragma once

#include "../ascene.h"

class Scene : public AbstractScene {
public:
	Scene(int width, int height);

	virtual void tick(float delta) override;

	virtual void draw(float delta) override;

	virtual void update(float delta) override;

	virtual void dispose() override;

	virtual void resize(int width, int height) override;

	virtual void addObject(SceneObject &object) override;
	virtual void addObject(Camera &camera, Size size);

	virtual void removeObject(SceneObject &object) override;

	//Camera utils
	SceneCameraConfig getCameraConfig(Camera &camera);
	void enableCamera(Camera &camera, bool enable);
	void configureCamera(Camera &camera, SceneCameraConfig config);

protected:
	//Meta

	Shader shader_basic;
	Shader shader_light;
	Shader shader_normals;
	Shader shader_geometry;



	GLuint gBufferFBO = 0;
	GLuint rboGDepth = 0;
	GLuint gBufferPosition = 0, gBufferNormal = 0, gBufferAlbedo = 0, gBufferOption1 = 0, gBloom = 0, gDepthStencil = 0;

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

	std::vector<SceneCamera> cameras;
	std::vector<Drawable*> objects;

	//lights
	DirectionalLight* directionalLight = nullptr;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

	std::vector<Animatable*> animatables;

	//Instancer
	std::vector<Instancer*> instancers;

	virtual void setup();
};