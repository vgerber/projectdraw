#pragma once

#include <vector>

#include "Core/Scene/UI/Components/uicomponent.h"
#include "Core/Scene/ascene.h"
#include "Core/Scene/Camera/orthocamera.h"
#include "Core/Renderer/FlatRenderer/flat.h"

class HUD : public AbstractScene {
public:

	HUD(int width, int height);

	/**
	 * @brief Clears scene width given color
	 * 
	 * @param r 
	 * @param g 
	 * @param b 
	 * @param a 
	 */
	virtual void clear(float r, float g, float b, float a);

	/**
	 * @brief Update @ref Animatable
	 * 
	 * @param delta 
	 */
	virtual void tick(float delta);

	/**
	 * @brief Draw all elements
	 * 
	 * @param delta 
	 */
	virtual void draw(float delta);

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
	virtual void dispose();

	/**
	 * @brief Sets new scene size in pixel
	 * 
	 * @param width 
	 * @param height 
	 */
	virtual void resize(int width, int height);

	/**
	 * @brief Add a @SceneObject
	 * 
	 * @param object 
	 */
	virtual void addObject(SceneObject &object);

	/**
	 * @brief Remove @SceneObject
	 * 
	 * @param object 
	 */
	virtual void removeObject(SceneObject &object);

	/**
	 * @brief Set the Background image
	 * 
	 * @param texture 
	 */
	void setBackground(unsigned int texture);

	/**
	 * @brief Get the rendered texture
	 * 
	 * @return unsigned int 
	 */
	unsigned int getTexture();

	/**
	 * @brief Draw texture to target framebuffer
	 * 
	 * @param framebuffer 
	 */
	void drawTexture(unsigned int framebuffer);

protected:
	OrthographicCamera * camera = nullptr;
	FlatRenderer * renderer = nullptr;


	std::vector<UIComponent*> uiComponents;

	std::vector<SceneObject*> components;

};