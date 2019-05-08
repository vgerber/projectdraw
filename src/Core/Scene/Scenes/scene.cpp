#include "scene.h"

Scene::Scene(int width, int height) : AbstractScene()
{
	setup();
	resize(width, height);
}

void Scene::clear(float r, float g, float b, float a) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glViewport(0, 0, getWidth(), getHeight());
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Scene::tick(float delta)
{
}

void Scene::draw(float delta)
{

	int width = getWidth();
	int height = getHeight();

	glDisable(GL_CULL_FACE);

	GLcheckError();
	for (auto &SubScene : subScenes)
	{
		//only draw active subScenes
		if (!SubScene.config.Active)
		{
			continue;
		}
		//clear and render subScene scene
		SubScene.renderer->clearScreen();
		SubScene.renderer->render();
	}
	GLcheckError();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	Shader shader = ResourceManager::loadShader(ShaderName::Renderer::Deferred::Pipeline::Texture::ScreenTexture);
	shader.use();
	glUniform1i(glGetUniformLocation(shader.getId(), "screenTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	for (auto SubScene : subScenes)
	{
		//only draw active subScenes
		if (!SubScene.config.Active)
		{
			continue;
		}
		SubScene.drawFrame(0);
	}
	glEnable(GL_DEPTH_TEST);
	if (hud) {
		glCopyTextureSubImage2D(sceneTexture, 0, 0, 0, 0, 0, width, height);
		hud->clear(0.0f, 0.0f, 0.0f, 0.0f);
		hud->setBackground(sceneTexture);
		hud->update(delta);
		hud->drawTexture(0);
	}
}

void Scene::update(float delta)
{
	tick(delta);
	draw(delta);
}

void Scene::dispose()
{
	for (auto subScene : subScenes)
	{
		subScene.dispose();
	}
}

void Scene::resize(int width, int height)
{
	AbstractScene::resize(width, height);

	glBindTexture(GL_TEXTURE_2D, sceneTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	GLcheckError();

	for (SubScene &subScene : subScenes)
	{
		subScene.resize(width, height);
	}

	if (hud) {
		hud->resize(width, height);
	}
}

void Scene::addObject(SceneObject &object)
{
	if (subScenes.size() == 0) {
		Log::write(LogType::Warning, "Scene has no renderers", "Scene");
	}
	for (auto subScene : subScenes) {
		subScene.renderer->addSceneObject(object);
	}
}

void Scene::addObject(SceneObject & object, AbstractRenderer & renderer)
{
	bool cameraFound = false;
	for (auto subScene : subScenes) {
		if (subScene.camera == renderer.getCamera()) {
			subScene.renderer->addSceneObject(object);
			cameraFound = true;
		}
	}
	if (!cameraFound) {
		Log::write(LogType::Error, "Target renderer does not exists", "Scene");
	}
}

void Scene::addSubScene(AbstractRenderer & renderer, Size size)
{
	subScenes.push_back(SubScene(renderer, size, getWidth(), getHeight()));
}

void Scene::setHUD(HUD & hud)
{
	this->hud = &hud;
}

void Scene::removeObject(SceneObject &object)
{
	for (auto subScene : subScenes) {
		subScene.renderer->removeSceneObject(object);
	}
}

void Scene::removeObject(SceneObject & object, AbstractRenderer &renderer)
{
	for (auto subScene : subScenes) {
		if (subScene.renderer == &renderer) {
			subScene.renderer->removeSceneObject(object);
		}
	}
}

void Scene::removeSubScene(AbstractRenderer & renderer) {
	subScenes.erase(std::remove_if(subScenes.begin(), subScenes.end(), [&renderer](SubScene & subScene) { return subScene.renderer == &renderer; }));
}

SubSceneConfig Scene::getSubSceneConfig(AbstractRenderer &renderer)
{
	for (size_t i = 0; i < subScenes.size(); i++)
	{
		if (subScenes[i].renderer == &renderer)
		{
			return subScenes[i].config;
		}
	}
	throw std::invalid_argument("subScene not found");
}

void Scene::enableSubScene(AbstractRenderer &renderer, bool enable)
{
	for (size_t i = 0; i < subScenes.size(); i++)
	{
		if (subScenes[i].renderer == &renderer)
		{
			subScenes[i].config.Active = enable;
		}
	}
}

void Scene::configureSubScene(AbstractRenderer &renderer, SubSceneConfig config)
{
	for (size_t i = 0; i < subScenes.size(); i++)
	{
		if (subScenes[i].renderer == &renderer)
		{
			subScenes[i].config = config;
		}
	}
}

void Scene::setup()
{
	glGenTextures(1, &sceneTexture);
}
