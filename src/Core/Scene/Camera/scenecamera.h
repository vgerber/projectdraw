#pragma once
#include "camera.h"
#include "../Primitive/primitives.h"
#include "../Renderer/r_deferred.h"
#include "../Renderer/arenderer.h"

struct SceneCameraConfig {
	bool debugVisible = true;
	bool dLightVisible = true;
	bool sLightVisible = true;
	bool pLightVisible = true;
	bool ParticleVisible = true;
	bool Active = true;
};


class SceneCamera {
public:
	//configuration
	SceneCameraConfig config;

	//actual camera
	Camera * camera;

	//scene renderer with camera as view
	AbstractRenderer * renderer = nullptr;



	SceneCamera(Camera &camera, Size frame, int sceneWidth, int sceneHeight);

	//get geometry for the camera viewfrustum (debug purpose)
	Geometry getDebugViewFrustum(int splits = 0);

	//clear renderer scene
	void clear();

	//get renderer texture
	GLuint getTexture();

	//dispose gl objects
	void dispose();

	//called when screen is resized and the renderer needs a new
	//size with aspect to its current frame
	void resize(int sceneWidth, int sceneHeight);

	//resize frame on screen
	//x/y: -1.0 - 1.0
	//width/height: 0.0 - 2.0
	void resizeFrame(Size frame);

	Size getFrame();

	void drawFrame(GLuint target);

	void setRendererOption(std::string option, bool value);

private:
	Size frame;
	int width, height;
	GLuint camVAO, camVBO;

	void setup(int sceneWidth, int sceneHeight);
	

};