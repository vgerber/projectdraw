#pragma once
#include "Core/Texture/texture.h"
#include "Core/Scene/Scenes/scene.h"
#include "Core/Scene/Camera/orthocamera.h"

/**
 * @brief Scene for image view
 * 
 */
class ImageScene : public AbstractScene {
public:
	/**
	 * @brief Construct a new Texture Scene object with screen size
	 * 
	 * @param width 
	 * @param height 
	 */
	ImageScene(int width, int height);
	
	/**
	 * @brief Update @ref Animatable
	 * 
	 * @param delta 
	 */
	virtual void tick(float delta) override;

	/**
	 * @brief Draw all textures
	 * 
	 * @param delta 
	 */
	virtual void draw(float delta) override;

	/**
	 * @brief Resize scene to size in pixel
	 * 
	 * @param width 
	 * @param height 
	 */
	virtual void resize(int width, int height) override;

	/**
	 * @brief Free allocated memory
	 * 
	 */
	virtual void dispose() override;

	/**
	 * @brief Add a @ref SceneObject to scene
	 * 
	 * @param object 
	 */
	virtual void addObject(SceneObject &object) override;

	/**
	 * @brief Add a new @ref Texture to scene
	 * 
	 * @param texture 
	 */
	virtual void addTexture(Texture &texture);

	/**
	 * @brief Remove @SceneObejct from scene
	 * 
	 * @param object 
	 */
	virtual void removeObject(SceneObject &object) override;

	/**
	 * @brief Remove @Texture from scene
	 * 
	 * @param texture 
	 */
	virtual void removeTexture(Texture &texture);
	

private:
	Texture * texture = nullptr;
	//Drawable textureHolder;

	OrthographicCamera * camera = nullptr;

	int screen_width = 0, screen_height = 0;
	float screen_aspect = 1.0f;

	GLuint tex_pos, view_mat_pos, proj_mat_pos, model_mat_pos;
	Shader shader;

	/**
	 * @brief Set the Texture object
	 * 
	 * @param texture 
	 */
	void setTexture(Texture *texture);

	/**
	 * @brief Setup internals
	 * 
	 */
	virtual void setup();


};