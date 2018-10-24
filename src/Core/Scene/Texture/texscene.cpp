#include "texscene.h"

TextureScene::TextureScene(int width, int height)
{
	setup();
	reload(width, height);
}

void TextureScene::draw(float delta)
{
	if (texture) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
}

void TextureScene::reload(int width, int height)
{
}

void TextureScene::setTexture(Texture & texture, float aspectRatio)
{
	this->texture = &texture;
	float width = texture.getWidth();
	float height = texture.getHeight();
	textureHolder.setModel(primitives::generateRectangle(width, height, glm::vec4(0.0f));

	if (!camera) {
		camera = new OrthographicCamera(glm::vec3(0.0f, 0.0f, 1.0f) glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		camera->FarZ = 2.0f;
		camera->NearZ = -2.0f;
	}

	camera->Width = width;
	camera->Height = aspectRatio * width;
}

void TextureScene::setup()
{
	Shader shader = ResourceManager::loadShader(SHADER_TEXTURE_BASIC);
	tex_pos = glGetUniformLocation(shader.getId(), "screenTexture");
	view_mat_pos = glGetUniformLocation(shader.getId(), "view");
	proj_mat_pos = glGetUniformLocation(shader.getId(), "projection");
	model_mat_pos = glGetUniformLocation(shader.getId(), "model");
}
