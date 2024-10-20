#include "sceneobject.h"



SceneObject::SceneObject()
{
}


SceneObject::~SceneObject()
{
}


std::string SceneObject::getId()
{
	return id;
}

void SceneObject::setId(std::string id)
{
	this->id = id;
}
