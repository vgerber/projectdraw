#pragma once

#include "../core.h"
#include "Camera/camera.h"
#include "drawable.h"
#include "Light/lights.h"

#include <map>
#include <memory>

class Scene
{
public:
	Scene();
	~Scene();

	void add_object(Drawable &drawable);
	void add_plight(PointLight &plight);

	void set_camera(Camera &camera);
	void set_dlight(DirectionalLight &dlight);

	virtual void draw(GLfloat delta);
	void dispose();
private:
	int width = 100, height = 100;

	GLuint gBufferFBO;
	GLuint gBufferPosition, gBufferNormal, gBufferAlbedo, gBufferUseLight;
	GLuint uboMatrices;
	Camera* camera = nullptr;
	std::vector<Drawable*> objects;

	//lights
	DirectionalLight* directional_light = nullptr;
	std::vector<PointLight*> point_lights;

	void setup(int width, int height);
};


