#pragma once


#include "../core.h"
#include "Camera/camera.h"
#include "Camera/scenecamera.h"
#include "drawable.h"
#include "Light/lights.h"
#include "Particle/particlegenerator.h"
#include "Instancing/instancer.h"
#include "Animation/animatable.h"

#include <map>
#include <memory>

struct SortDrawable {
	glm::vec3 cameraPosition;

	SortDrawable(glm::vec3 cameraPosition) { this->cameraPosition = cameraPosition; }

	inline bool operator() (Drawable *d1, Drawable *d2) {
		if (!d1->settings.xrayVisible && d2->settings.xrayVisible) {
			return true;
		}
		if (glm::distance2(d1->getPositionCenter(), cameraPosition) > glm::distance2(d2->getPositionCenter(), cameraPosition)) {
			return true;
		}
		return false;
	}
};


enum RenderMode {
	POINTR = 0,
	LINER = 1,
	FILLR = 2
};

class Scene
{
public:
	RenderMode renderMode;

	Scene();
	Scene(int width, int height);
	~Scene();

	void addDrawable(Drawable &drawable);
	void addAnimatable(Animatable &animatable);
	void addInstancer(Instancer &instancer);

	void addPlight(PointLight &plight);
	void addSLight(SpotLight &sLight);


	void removeDrawable(Drawable &drawable);
	void removeAnimatable(Animatable &animatable);

	void removeCamera(Camera &camera);
	void enableCamera(Camera &camera, bool enable);
	void configureCamera(Camera &camera, SceneCameraConfig config);
	SceneCameraConfig getCameraConfig(Camera &camera);

	//camera with size from -1.0 - 1.0
	void addCamera(Camera &camera, Size size);
	void setDlight(DirectionalLight &dlight);


	virtual void draw(GLfloat delta);

	void dispose();

	virtual void reload(int width, int height);


private:
	static int globalSceneId;
	int sceneID;
	
protected:
	int width = 100, height = 100;

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

	int getSceneId();


};


