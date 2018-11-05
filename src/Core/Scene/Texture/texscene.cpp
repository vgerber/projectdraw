#include "texscene.h"

TextureScene::TextureScene(int width, int height)
{
	setup();
	reload(width, height);
}

void TextureScene::draw(float delta)
{
	if (texture || true) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.2f, 0.7f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glViewport(0, 0, screen_width, screen_height);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_CULL_FACE);
		

		shader.use();
		glUniformMatrix4fv(view_mat_pos, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
		glUniformMatrix4fv(proj_mat_pos, 1, GL_FALSE, glm::value_ptr(camera->getCameraMatrix()));
		
		glUniform1i(tex_pos, 0);
		texture->activate(0);
		

		textureHolder.draw(shader);
	}
}

void TextureScene::reload(int width, int height)
{
	if (screen_width != width || screen_height != height || true) {
		screen_width = width;
		screen_height = height;
		screen_aspect = ((float)height / (float)width);

		if (!camera) {
			camera = new OrthographicCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			camera->FarZ = 2.0f;
			camera->NearZ = -2.0f;
		}
		float objWidth = textureHolder.getSize().width;
		float objHeight = textureHolder.getSize().height;

		float camHeight;
		float camWidth;

		float nw = objHeight * screen_width / screen_height;
		float nh = objWidth * screen_height / screen_width;
		if (nw >= objWidth) {
			camWidth = nw;
			camHeight = objHeight;
		}
		else {
			camWidth = objWidth;
			camHeight = nh;
		}
		
		camera->Width = camWidth;
		camera->Height = camHeight;
	}
}

void TextureScene::setTexture(Texture *texture)
{
	float width = this->texture ? this->texture->getWidth() : 0.0f;
	float height = this->texture ? this->texture->getHeight() : 0.0f;
	this->texture = texture;

	bool reloadSize = (texture->getWidth() != width || texture->getHeight() != height);



	textureHolder.setModel(
		primitives::generateRectangle(texture->getWidth(), texture->getHeight(), glm::vec4(1.0f))
	);
	textureHolder.setPositionCenter(glm::vec3(0.0f, 0.0f, 0.0f));
	if(reloadSize) 
		reload(screen_width, screen_height);
}

void TextureScene::setup()
{
	shader = ResourceManager::loadShader(SHADER_TEXTURE_BASIC);
	shader.use();
	tex_pos = glGetUniformLocation(shader.getId(), "texture1");
	view_mat_pos = glGetUniformLocation(shader.getId(), "view");
	proj_mat_pos = glGetUniformLocation(shader.getId(), "projection");
	model_mat_pos = glGetUniformLocation(shader.getId(), "model");
}
