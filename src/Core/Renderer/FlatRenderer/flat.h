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
	unsigned int rendererTexture;
	unsigned int rendererFBO;
	unsigned int rendererRBO;
	unsigned int multisampleRendererTexture;
	unsigned int multisampleRendererFBO;
	unsigned int multisampleRendererRBO;

	unsigned int screenRectVBO = 0;
	unsigned int screenRectVAO = 0;

	std::vector<Drawable*> drawables;

	Shader shaderMesh;
	Shader shaderTexture;

	virtual void setup();

private:
	///Draw drawable as plain geometry (without features)
	virtual void renderDrawableRaw(Drawable * drawable, Shader shader, DrawType drawType = DrawType::TRIANGLEG);

	unsigned int msaa = 0;
};