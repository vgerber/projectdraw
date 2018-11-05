#include "ascene.h"

int AbstractScene::globalSceneId = 0;

AbstractScene::AbstractScene(int width, int height)
{
	this->width = width;
	this->height = height;
	this->sceneID = globalSceneId++;
}

void AbstractScene::resize(int width, int height)
{
	this->width = width;
	this->height = height;
}

Size AbstractScene::getSize()
{
	return Size() = { 0.0f, 0.0f, 0.0f, width, height, 0.0f };
}

int AbstractScene::getWidth()
{
	return width;
}

int AbstractScene::getHeight()
{
	return height;
}

int AbstractScene::getSceneId()
{
	return sceneID;
}
