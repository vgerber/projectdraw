#pragma once
#include "texture.h"
#include "../scene.h"
#include "../Camera/orthocamera.h"

class TextureScene : public Scene {
public:
	TextureScene(int width, int height);
	
	virtual void draw(float delta) override;
	virtual void reload(int width, int height) override;

	void setTexture(Texture &texture, float aspectRation = 1.0f);

private:
	Texture * texture;
	Drawable textureHolder;

	OrthographicCamera * camera;

	GLuint tex_pos, view_mat_pos, proj_mat_pos, model_mat_pos;

	virtual void setup() override;
};