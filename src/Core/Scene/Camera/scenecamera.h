#pragma once
#include "camera.h"
#include "../Primitive/primitives.h"

struct SceneCameraConfig {
	bool debugVisible = true;
	bool dLightVisible = true;
	bool slightVisible = true;
	bool pLightVisible = true;
	bool ParticleVisible = true;
	bool Active = true;
};


class SceneCamera {
public:
	SceneCameraConfig config;

	SceneCamera(Camera &camera, Size frame, int sceneWidth, int sceneHeight);

	Camera * camera;
	Size frame;

	void beginDrawing(Shader shader);
	Geometry getDebugViewFrustum(int splits = 0);
	void clear();
	void endDrawing();

	GLuint getTexture();
	GLuint getFrameVerticesVAO();

	void setFrame(Size frame);

	void dispose();

private:
	float Width = 0.0f, Height = 0.0f;
	GLuint sceneTexture, VBO, VAO, FBO;

	void setup(int sceneWidth, int sceneHeight);
	void reload(int sceneWidth, int sceneHeight);

};