#include <iostream>

#include "Core/core.h"
#include "Core/Scene/Camera/camera.h"
#include "Core/Scene/Camera/scenecamera.h"
#include "Core/Scene/drawable.h"
#include "Core/Scene/Light/lights.h"
#include "Core/Scene/Particle/particlegenerator.h"
#include "Core/Scene/Instancing/instancer.h"
#include "Core/Scene/Animation/animatable.h"
#include "Core/Scene/Text/text.h"
#include "Core/Texture/Filter/HDR/hdr.h"

#include "Core/Renderer/arenderer.h"

#include "Core/Renderer/FlatRenderer/Interface/flatsceneobject.h"
#include "Core/Renderer/FlatRenderer/Interface/flatdrawable.h"
#include "Core/Renderer/FlatRenderer/Interface/flatmesh.h"
#include "Core/Renderer/FlatRenderer/Interface/flattext.h"


/**
 * @brief 2D Renderer
 * 
 */
class FlatRenderer : public AbstractRenderer {
public:

	/**
	 * @brief Construct a new Flat Renderer object
	 * 
	 * @param width 
	 * @param height 
	 * @param camera 
	 */
	FlatRenderer(int width, int height, Camera &camera);

	/**
	 * @brief Resize internals to size
	 * 
	 * @param width 
	 * @param height 
	 */
	virtual void resize(int width, int height) override;

	/**
	 * @brief Clear scene textures
	 * 
	 */
	virtual void clearScreen() override;

	/**
	 * @brief Set the Background texture
	 * 
	 * @param texture 
	 */
	virtual void setBackground(unsigned int texture);

	/**
	 * @brief Render objects to scene
	 * 
	 */
	virtual void render() override;

	/**
	 * @brief Add an object to render process
	 * 
	 * @param sceneObject 
	 */
	virtual void addSceneObject(SceneObject &sceneObject) override;

	/**
	 * @brief Remove object from render process
	 * 
	 * @param sceneObject 
	 */
	virtual void removeSceneObject(SceneObject &sceneObject) override;

	/**
	 * @brief Get the final render texture
	 * 
	 * @return GLuint 
	 */
	virtual GLuint getTexture() override;

	/**
	 * @brief Free allocated resources
	 * 
	 */
	virtual void dispose() override;

	/**
	 * @brief Render scene texture to new framebuffer
	 * 
	 * Both have to be the same size
	 * 
	 * @param framebuffer 
	 */
	virtual void drawTexture(unsigned int framebuffer);

protected:
	unsigned int rendererTexture;				//final texture after render process
	unsigned int rendererFBO;					//framebuffer for rendering
	unsigned int rendererRBO;					//render buffer object for rendering
	unsigned int multisampleRendererTexture;	//multisample option
	unsigned int multisampleRendererFBO;		
	unsigned int multisampleRendererRBO;

	unsigned int screenRectVBO = 0;				//vertex buffer obejct for screenspace texture
	unsigned int screenRectVAO = 0;				//vertex array object for vbo storage

	std::vector<FlatSceneObject*> sceneObjects;	//added drawables

	Shader shaderMesh;							//shader for mesh rendering
	Shader shaderTexture;						//shader for render from texture

	/**
	 * @brief Setup internal opengl objects
	 * 
	 */
	virtual void setup();

private:
	/**
	 * @brief Generates an suitable flat object wrapper for the @ref SceneObject structure
	 * 
	 * @param sceneObject 
	 * @return FlatSceneObject* 
	 */
	FlatSceneObject * generateFlatObject(SceneObject * sceneObject);


	unsigned int msaa = 4;
};