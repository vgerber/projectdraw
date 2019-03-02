#pragma once

#include <vector>

#include "Core/Scene/UI/Components/uicomponent.h"
#include "Core/Scene/ascene.h"
#include "Core/Scene/Camera//orthocamera.h"
#include "Core/Renderer/FlatRenderer/flat.h"

class HUD : public AbstractScene {
public:

	HUD(int width, int height);

	/*
	*clears scene with color
	*/
	virtual void clear(float r, float g, float b, float a);

	/*
	*update timed objects
	*/
	virtual void tick(float delta);

	/*
	*draw objects
	*/
	virtual void draw(float delta);

	/*
	*update and draw objects
	*/
	virtual void update(float delta);

	/*
	*free memory
	*/
	virtual void dispose();

	/*
	*set new scene size (viewport).
	*call when window size is changed.
	*/
	virtual void resize(int width, int height);

	/*
	*add object to scene
	*object: added object
	*camera: target camera which handles the added object
	*/
	virtual void addObject(SceneObject &object, Camera &camera);

	/*
	*add object to all currently added cameras
	*/
	virtual void addObject(SceneObject &object);

	/*
	*remove object from scene target camera
	*/
	virtual void removeObject(SceneObject &object, Camera &camera);

	/*
	*remove object from all cameras
	*/
	virtual void removeObject(SceneObject &object);

	void setBackground(unsigned int texture);

	unsigned int getTexture();

	/*
	* draw renderer texture to target framebuffer
	*/
	void drawTexture(unsigned int framebuffer);

protected:
	OrthographicCamera * camera = nullptr;
	FlatRenderer * renderer = nullptr;


	std::vector<UIComponent*> uiComponents;

	std::vector<SceneObject*> components;

};