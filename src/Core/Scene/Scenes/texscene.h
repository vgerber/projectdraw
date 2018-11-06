#pragma once
#include "../Texture/texture.h"
#include "../Scenes/scene.h"
#include "../Camera/orthocamera.h"

class TextureScene : public AbstractScene {
public:
	TextureScene(int width, int height);
	
	virtual void tick(float delta) override;
	virtual void draw(float delta) override;
	virtual void resize(int width, int height) override;
	virtual void dispose() override;

	virtual void addObject(SceneObject &object) override;
	virtual void addTexture(Texture &texture);
	virtual void removeObject(SceneObject &object) override;
	virtual void removeTexture(Texture &texture);
	

private:
	Texture * texture = nullptr;
	Drawable textureHolder;

	OrthographicCamera * camera = nullptr;

	int screen_width = 0, screen_height = 0;
	float screen_aspect = 1.0f;

	GLuint tex_pos, view_mat_pos, proj_mat_pos, model_mat_pos;
	Shader shader;

	void setTexture(Texture *texture);
	virtual void setup();


};