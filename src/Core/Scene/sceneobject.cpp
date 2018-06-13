#include "sceneobject.h"



SceneObject::SceneObject()
{
}


SceneObject::~SceneObject()
{
}

glm::vec3 SceneObject::get_position()
{
	return position;
}

std::string SceneObject::get_id()
{
	return id;
}

void SceneObject::set_position(glm::vec3 position)
{
	this->position = position;
}

void SceneObject::set_id(std::string id)
{
	this->id = id;
}
