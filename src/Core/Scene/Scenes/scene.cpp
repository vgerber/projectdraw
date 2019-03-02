#include "scene.h"

Scene::Scene(int width, int height) : AbstractScene(width, height)
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
	for (auto &sceneCamera : cameras)
	{
		//only draw active cameras
		if (!sceneCamera.config.Active)
		{
			continue;
		}
		//clear and render camera scene
		sceneCamera.renderer->clearScreen();
		sceneCamera.renderer->render();
	}
	GLcheckError();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	Shader shader = ResourceManager::loadShader(ShaderName::Renderer::Deferred::Pipeline::Texture::ScreenTexture);
	shader.use();
	glUniform1i(glGetUniformLocation(shader.getId(), "screenTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	for (auto sceneCamera : cameras)
	{
		//only draw active cameras
		if (!sceneCamera.config.Active)
		{
			continue;
		}
		sceneCamera.drawFrame(0);
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
	for (auto camera : cameras)
	{
		camera.dispose();
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

	for (SceneCamera &camera : cameras)
	{
		camera.resize(width, height);
	}

	if (hud) {
		hud->resize(width, height);
	}
}

void Scene::addObject(SceneObject &object)
{
	if (Camera * camera = dynamic_cast<Camera*>(&object)) {
		addObject(*camera, Size(-1.0f, -1.0f, 0.0f, 2.0f, 2.0f, 0.0f));
		return;
	}
	if (cameras.size() == 0) {
		printf("Scene [Error]  scene has no cameras\n");
	}
	for (auto camera : cameras) {
		camera.renderer->addSceneObject(object);
	}
}

void Scene::addObject(SceneObject & object, Camera & camera)
{
	bool cameraFound = false;
	for (auto cam : cameras) {
		if (cam.camera == &camera) {
			cam.renderer->addSceneObject(object);
			cameraFound = true;
		}
	}
	if (!cameraFound) {
		printf("Scene ERR - Target camera does not exist in scene\n");
	}
}

void Scene::addObject(Camera &camera, Size size)
{
	cameras.push_back(SceneCamera(camera, size, getWidth(), getHeight()));
}

void Scene::setHUD(HUD & hud)
{
	this->hud = &hud;
}

void Scene::removeObject(SceneObject &object)
{
	if (Camera *camera = dynamic_cast<Camera *>(&object))
	{
		for (size_t i = 0; i < cameras.size(); i++)
		{
			if (cameras[i].camera == camera)
			{
				cameras[i].dispose();
				cameras.erase(cameras.begin() + i);
				i--;
			}
		}
	}

	bool found = false;
	for (auto camera : cameras) {
		camera.renderer->removeSceneObject(object);
	}

	//printf("[Engine] [Scene] [Error] Scene doesn't accept %s\n", object.getId().c_str());
}

void Scene::removeObject(SceneObject & object, Camera & camera)
{

	for (auto cam : cameras) {
		if (cam.camera == &camera) {
			cam.renderer->removeSceneObject(object);
		}
	}

	printf("[Engine] [Scene] [Error] Scene doesn't accept %s\n", object.getId().c_str());
}

SceneCameraConfig Scene::getCameraConfig(Camera &camera)
{
	for (size_t i = 0; i < cameras.size(); i++)
	{
		if (cameras[i].camera == &camera)
		{
			return cameras[i].config;
		}
	}
	throw std::invalid_argument("Camera not found");
}

void Scene::enableCamera(Camera &camera, bool enable)
{
	for (size_t i = 0; i < cameras.size(); i++)
	{
		if (cameras[i].camera == &camera)
		{
			cameras[i].config.Active = enable;
		}
	}
}

void Scene::configureCamera(Camera &camera, SceneCameraConfig config)
{
	for (size_t i = 0; i < cameras.size(); i++)
	{
		if (cameras[i].camera == &camera)
		{
			cameras[i].config = config;
		}
	}
}

void Scene::setup()
{
	glGenTextures(1, &sceneTexture);
}
