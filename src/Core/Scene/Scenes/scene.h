#pragma once

#include "Core/Scene/ascene.h"
#include "Core/Scene/UI/HUD/hud.h"

/**
 * @brief Container for multiple rendering and display
 * 
 * Each scene has @ref SubScene with an own renderer
 * On draw each renderer will perform a render action or not
 * and will renderer his texture to the final scene
 * 
 * Each @ref SubScene can be identified be the Renderer address 
 */
class Scene : public AbstractScene {
public:
	Scene(int width, int height);

	virtual void clear(float r, float g, float b, float a);

	virtual void tick(float delta) override;

	virtual void draw(float delta) override;

	virtual void update(float delta) override;

	virtual void dispose() override;

	virtual void resize(int width, int height) override;

	virtual void addObject(SceneObject &object) override;
	virtual void addObject(SceneObject &object, AbstractRenderer & renderer) override;

	/**
	 * @brief Adds a renderer (subscene) to the draw loop
	 * 
	 * @param renderer 
	 * @param size 
	 */
	virtual void addSubScene(AbstractRenderer & renderer, Size size);

	/**
	 * @brief Set heads up display for scene
	 * 
	 * Each scene can only have one hud
	 */
	virtual void setHUD(HUD &hud);

	/**
	 * @brief Remove object from all subscenes
	 * 
	 * @param object 
	 */
	virtual void removeObject(SceneObject &object) override;

	/**
	 * @brief Remove object from specific subscene
	 * 
	 * @param object 
	 * @param camera 
	 */
	virtual void removeObject(SceneObject &object, AbstractRenderer & renderer) override;

	/**
	 * @brief Removes subscene from scene
	 * 
	 * @param renderer 
	 */
	virtual void removeSubScene(AbstractRenderer &renderer);

	/**
	 * @brief Get the Camera Config
	 * 
	 * @param camera 
	 * @return SceneCameraConfig 
	 */
	SubSceneConfig getSubSceneConfig(AbstractRenderer &renderer);
	void enableSubScene(AbstractRenderer &renderer, bool enable);
	void configureSubScene(AbstractRenderer &renderer, SubSceneConfig config);

protected:
	std::vector<SubScene> subScenes;

	HUD * hud = nullptr;
	unsigned int sceneTexture;

	virtual void setup();
};