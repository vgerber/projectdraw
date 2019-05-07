#include "hud.h"

HUD::HUD(int width, int height) : AbstractScene(width, height)
{
	camera = new OrthographicCamera(glm::vec3(width * 0.5f, height * 0.5f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera->setClipping(0.1f, 1000.0f);	
	renderer = new FlatRenderer(width, height, *camera);
	resize(width, height);
}

void HUD::clear(float r, float g, float b, float a)
{
	renderer->clearScreen();
}

void HUD::tick(float delta)
{
	for (auto uiComponent : uiComponents) {
		uiComponent->update();
	}
}

void HUD::draw(float delta)
{
	renderer->render();
}

void HUD::update(float delta)
{
	tick(delta);
	draw(delta);
}

void HUD::dispose()
{
	renderer->dispose();
}

void HUD::resize(int width, int height)
{
	camera->setSize(width, height);
	renderer->resize(width, height);	
}

void HUD::addObject(SceneObject & object, AbstractRenderer &renderer)
{
	addObject(object);
}

void HUD::addObject(SceneObject & object)
{
	renderer->addSceneObject(object);
	components.push_back(&object);
	if (UIComponent * uicomponent = dynamic_cast<UIComponent*>(&object)) {
		uiComponents.push_back(uicomponent);
	}
}

void HUD::removeObject(SceneObject & object, AbstractRenderer &renderer)
{
	removeObject(object);
}

void HUD::removeObject(SceneObject & object)
{
	for (int i = 0; i < components.size(); i++) {
		if (&object == components[i]) {
			components.erase(components.begin() + i);
			renderer->removeSceneObject(object);
			for (int j = 0; j < uiComponents.size(); j++) {
				if (&object == uiComponents[j]) {
					uiComponents.erase(uiComponents.begin() + j);
				}
			}
		}
	}
}

void HUD::setBackground(unsigned int texture)
{
	renderer->setBackground(texture);
}

unsigned int HUD::getTexture()
{
	return renderer->getTexture();
}

void HUD::drawTexture(unsigned int framebuffer)
{
	renderer->drawTexture(framebuffer);
}
