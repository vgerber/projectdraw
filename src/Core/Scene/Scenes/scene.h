#pragma once

#include "Core/Scene/ascene.h"
#include "Core/Scene/UI/HUD/hud.h"

class Scene : public AbstractScene {
public:
	Scene(int width, int height);

	/*
	*clears scene with color
	*/
	virtual void clear(float r, float g, float b, float a);

	virtual void tick(float delta) override;

	virtual void draw(float delta) override;

	virtual void update(float delta) override;

	virtual void dispose() override;

	virtual void resize(int width, int height) override;

	virtual void addObject(SceneObject &object) override;
	virtual void addObject(SceneObject &object, Camera &camera) override;
	virtual void addObject(Camera &camera, Size size);

	/*
	* Set heads up display for scene#
	* Each scene can only have one hud
	*/
	virtual void setHUD(HUD &hud);

	virtual void removeObject(SceneObject &object) override;
	virtual void removeObject(SceneObject &object, Camera &camera) override;

	//Camera utils
	SceneCameraConfig getCameraConfig(Camera &camera);
	void enableCamera(Camera &camera, bool enable);
	void configureCamera(Camera &camera, SceneCameraConfig config);

protected:

	std::vector<SceneCamera> cameras;

	HUD * hud = nullptr;
	unsigned int sceneTexture;

	virtual void setup();
};