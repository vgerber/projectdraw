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
	SceneCameraConfig config;
	Camera * camera;
	AbstractRenderer * renderer = nullptr;



	SceneCamera(Camera &camera, Size frame, int sceneWidth, int sceneHeight);



	Size frame;

	void beginDrawing(Shader shader);
	Geometry getDebugViewFrustum(int splits = 0);
	void clear();
	void endDrawing();

	GLuint getTexture();
	GLuint getFrameVerticesVAO();

	void setFrame(Size frame);

	void dispose();
	void resize(int sceneWidth, int sceneHeight);

private:
	float Width = 0.0f, Height = 0.0f;
	GLuint sceneTexture, VBO, VAO, FBO;

	void setup(int sceneWidth, int sceneHeight);
	

};