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

	/**
	 * @brief Construct a new Abstract Scene object
	 */
	AbstractScene();

	/**
	 * @brief Update @ref Animatable
	 * 
	 * @param delta 
	 */
	virtual void tick(float delta) = 0;

	/**
	 * @brief Draw all @ref SceneObject
	 * 
	 * @param delta 
	 */
	virtual void draw(float delta) = 0;

	/**
	 * @brief Calls tick and draw
	 * 
	 * @param delta 
	 */
	virtual void update(float delta);

	/**
	 * @brief Free allocated memory
	 * 
	 */
	virtual void dispose() = 0;

	/**
	 * @brief Resize internals to new screen size
	 * 
	 * @param width 
	 * @param height 
	 */
	virtual void resize(int width, int height);

	/**
	 * @brief Add @ref SceneObejct to scene
	 * 
	 * @param object 
	 */
	virtual void addObject(SceneObject &object) = 0;

	/**
	 * @brief Remove @ref SceneObject from scene
	 * 
	 * @param object 
	 */
	virtual void removeObject(SceneObject &object) = 0;

	/**
	 * @brief Get the scene Size in pixel
	 * 
	 * @return Size 
	 */
	Size getSize();

	/**
	 * @brief Get the scene Width in pixel
	 * 
	 * @return int 
	 */
	int getWidth();

	/**
	 * @brief Get the Height in pixel
	 * 
	 * @return int 
	 */
	int getHeight();

private:
	static int globalSceneId;
	int sceneID;
	int width = 0, height = 0;

protected:
	/**
	 * @brief Get the unique Scene id
	 * 
	 * @return int 
	 */
	int getSceneId();
};
