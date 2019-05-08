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
	/**
	 * @brief Construct a new Scene object 
	 * 
	 * @param width  Screen width
	 * @param height Screen height
	 */
	Scene(int width, int height);

	/**
	 * @brief Clear all @ref SubScene with given color
	 * 
	 * @param r 
	 * @param g 
	 * @param b 
	 * @param a 
	 */
	virtual void clear(float r, float g, float b, float a);

	/**
	 * @brief Update all animatables
	 * 
	 * @param delta 
	 */
	virtual void tick(float delta) override;

	/**
	 * @brief Draw on all active subscenes
	 * 
	 * @param delta 
	 */
	virtual void draw(float delta) override;

	/**
	 * @brief Calls tick and draw
	 * 
	 * @param delta 
	 */
	virtual void update(float delta) override;

	/**
	 * @brief Dispose all subscenes and internels
	 * 
	 */
	virtual void dispose() override;

	/**
	 * @brief Resize internal buffers to new screen size
	 * 
	 * @param width 
	 * @param height 
	 */
	virtual void resize(int width, int height) override;

	/**
	 * @brief Adds a @ref SceneObject to all @ref SubScenes
	 * 
	 * @param object 
	 */
	virtual void addObject(SceneObject &object) override;

	/**
	 * @brief Adds a @ref SceneObject to a specific @ref SubScene
	 * 
	 * @param object 
	 * @param renderer 
	 */
	virtual void addObject(SceneObject &object, AbstractRenderer & renderer);

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
	virtual void removeObject(SceneObject &object, AbstractRenderer & renderer);

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

	/**
	 * @brief Enable/Disable a @ref Subscene from tick/draw events
	 * 
	 * @param renderer 
	 * @param enable 
	 */
	void enableSubScene(AbstractRenderer &renderer, bool enable);

	/**
	 * @brief Sets the given config to a specific @ref SubScene
	 * 
	 * @param renderer 
	 * @param config 
	 */
	void configureSubScene(AbstractRenderer &renderer, SubSceneConfig config);

protected:
	std::vector<SubScene> subScenes;

	HUD * hud = nullptr;
	unsigned int sceneTexture;

	virtual void setup();
};