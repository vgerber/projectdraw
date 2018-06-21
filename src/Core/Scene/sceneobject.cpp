#include "sceneobject.h"



SceneObject::SceneObject()
{
}


SceneObject::~SceneObject()
{
}

glm::vec3 SceneObject::getPosition()
{
	return position;
}

std::string SceneObject::getId()
{
	return id;
}

void SceneObject::setPosition(glm::vec3 position)
{
	this->position = position;
}

void SceneObject::setId(std::string id)
{
	this->id = id;
}
