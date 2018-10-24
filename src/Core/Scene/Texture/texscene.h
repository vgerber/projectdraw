#pragma once
#include "texture.h"
#include "../scene.h"
#include "../Camera/orthocamera.h"

class TextureScene : public Scene {
public:
	TextureScene(int width, int height);
	
	virtual void draw(float delta) override;
	virtual void reload(int width, int height) override;

	void setTexture(Texture &texture);

private:
	Texture * texture;
	Drawable textureHolder;

	OrthographicCamera * camera = nullptr;

	int screen_width = 0, screen_height = 0;
	float screen_aspect = 1.0f;

	GLuint tex_pos, view_mat_pos, proj_mat_pos, model_mat_pos;
	Shader shader;

	virtual void setup() override;
};