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

class AbstractScene
{
public:
	RenderMode renderMode;

	AbstractScene(int width, int height);

	/*
	draw and update objects
	*/
	virtual void tick(float delta) = 0;

	/*
	free memory
	*/
	virtual void dispose() = 0;

	/*
	set new scene size (viewport).
	call when window size is changed.
	*/
	virtual void resize(int width, int height);

	/*
	add object to scene
	*/
	virtual void addObject(SceneObject &object) = 0;

	/*
	remove object from scene
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


