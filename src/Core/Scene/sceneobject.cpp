#include "sceneobject.h"



SceneObject::SceneObject()
{
}


SceneObject::~SceneObject()
{
}

glm::vec3 SceneObject::GetPosition()
{
	return position;
}

std::string SceneObject::GetId()
{
	return id;
}

void SceneObject::SetPosition(glm::vec3 position)
{
	this->position = position;
}

void SceneObject::SetId(std::string id)
{
	this->id = id;
}
