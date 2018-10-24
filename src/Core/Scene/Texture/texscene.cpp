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
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glViewport(0, 0, screen_width, screen_height);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		

		shader.use();
		glUniformMatrix4fv(view_mat_pos, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
		glUniformMatrix4fv(proj_mat_pos, 1, GL_FALSE, glm::value_ptr(camera->getCameraMatrix()));
		glUniform1i(tex_pos, texture->getGLTexture());
		texture->activate(0);

		textureHolder.draw(shader);
	}
}

void TextureScene::reload(int width, int height)
{
	screen_width = width;
	screen_height = height;
	screen_aspect = ((float)height / (float)width);

	if (!camera) {
		camera = new OrthographicCamera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		camera->FarZ = 2.0f;
		camera->NearZ = -2.0f;
	}

	camera->Width = width;
	camera->Height = screen_aspect * width;
}

void TextureScene::setTexture(Texture & texture)
{
	this->texture = &texture;
	float width = texture.getWidth();
	float height = texture.getHeight();
	textureHolder.setModel(primitives::generateRectangle(width, height, glm::vec4(0.0f)));

	if (!camera) {
		camera = new OrthographicCamera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		camera->FarZ = 2.0f;
		camera->NearZ = -2.0f;
	}

	camera->Width = width;
	camera->Height = screen_aspect * width;
}

void TextureScene::setup()
{
	shader = ResourceManager::loadShader(SHADER_TEXTURE_BASIC);
	tex_pos = glGetUniformLocation(shader.getId(), "screenTexture");
	view_mat_pos = glGetUniformLocation(shader.getId(), "view");
	proj_mat_pos = glGetUniformLocation(shader.getId(), "projection");
	model_mat_pos = glGetUniformLocation(shader.getId(), "model");
}
