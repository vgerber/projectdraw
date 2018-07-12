#pragma once
#include "camera.h"

class SceneCamera {
public:
	bool dLight = true;
	bool slight = true;
	bool pLight = true;
	bool Particle = true;

	bool active = true;


	SceneCamera(Camera &camera, Size frame);

	Camera * camera;
	Size frame;

	void beginDrawing(Shader shader);
	void endDrawing();

	GLuint getTexture();
	GLuint getFrameVerticesVAO();

	void setFrame(Size frame);

	void dispose();

private:
	float Width = 0.0f, Height = 0.0f;
	GLuint sceneTexture, VBO, VAO, FBO;

	void setup();
	void reload();

};