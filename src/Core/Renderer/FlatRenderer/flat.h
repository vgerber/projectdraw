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

/*
* FlatRenderer is a basic 2D renderer
*/
class FlatRenderer : public AbstractRenderer {
public:
	FlatRenderer(int width, int height, Camera &camera);

	/*
	* Resize internal storage
	*/
	virtual void resize(int width, int height) override;

	/*
	* Clear render buffers
	*/
	virtual void clearScreen() override;

	/*
	* Set background texture
	*/
	virtual void setBackground(unsigned int texture);

	/*
	* Render meshes
	*/
	virtual void render() override;

	/*
	* Add object to the render qeue
	*/
	virtual void addSceneObject(SceneObject &sceneObject) override;

	/*
	* Remove object from render qeue
	*/
	virtual void removeSceneObject(SceneObject &sceneObject) override;

	/*
	* Get texture with render() results
	*/
	virtual GLuint getTexture() override;

	/*
	* Free memory
	*/
	virtual void dispose() override;

	/*
	* Draw rendered image to a new framebuffer
	* Both buffers have to be the same size
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

	std::vector<Drawable*> drawables;			//added drawables

	Shader shaderMesh;							//shader for mesh rendering
	Shader shaderTexture;						//shader for render from texture

	/**
	 * @brief Setup internal opengl objects
	 * 
	 */
	virtual void setup();

private:
	/**
	 * @brief render drawable without special @ref
	 * 
	 */
	virtual void renderDrawableRaw(Drawable * drawable, Shader shader, DrawType drawType = DrawType::TRIANGLEG);

	unsigned int msaa = 0;
};