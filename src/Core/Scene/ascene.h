#pragma once


#include "Core/core.h"
#include "Core/Scene/Camera/camera.h"
#include "Core/Scene/Scenes/subscene.h"
#include "Core/Scene/drawable.h"
#include "Core/Scene/Light/lights.h"
#include "Core/Scene/Particle/particles.h"
#include "Core/Scene/Animation/animatable.h"

#include <map>
#include <memory>


class AbstractScene
{
public:
	RenderMode renderMode;

	AbstractScene(int width, int height);

	/*
	*clears scene with color
	*/
	virtual void clear(float r, float g, float b, float a) = 0;

	/*
	*update timed objects
	*/
	virtual void tick(float delta) = 0;

	/*
	*draw objects
	*/
	virtual void draw(float delta) = 0;

	/*
	*update and draw objects
	*/
	virtual void update(float delta);

	/*
	*free memory
	*/
	virtual void dispose() = 0;

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
	virtual void addObject(SceneObject &object, AbstractRenderer & renderer) = 0;

	/*
	*add object to all currently added cameras	
	*/
	virtual void addObject(SceneObject &object) = 0;

	/*
	*remove object from scene target camera
	*/
	virtual void removeObject(SceneObject &object, AbstractRenderer & renderer) = 0;

	/*
	*remove object from all cameras
	*/
	virtual void removeObject(SceneObject &object) = 0;

	Size getSize();
	int getWidth();
	int getHeight();

private:
	static int globalSceneId;
	int sceneID;
	int width = 0, height = 0;
protected:
	int getSceneId();
};


