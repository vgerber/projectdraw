#include "sceneobject.h"



SceneObject::SceneObject()
{
}


SceneObject::~SceneObject()
{
}

SceneObject * SceneObject::getChild(std::string id) {
	if(this->id == id) {
		return this;
	}
	for(auto child : children) {
		SceneObject * result = child->getChild(id);
		if(result) {
			return result;
		}
	}
	return nullptr;
}

void SceneObject::addChild(SceneObject * sceneObject) {
	children.push_back(sceneObject);
	//update transforms for new child
	transformChanged();
}

std::vector<SceneObject*> SceneObject::getChildren() {
	return children;
}

void SceneObject::removeChild(SceneObject * sceneObject, bool full) {
	bool childFound = false;

	for(auto child : children) {
		if(child == sceneObject) {
			childFound = true;
			break;
		}
	}

	if(!childFound) {
		return;
	}

	if(full) {
		for(auto child : sceneObject->getChildren()) {
			sceneObject->removeChild(child, full);
		}
		std::remove_if(children.begin(), children.end(), [sceneObject](SceneObject * child) { return sceneObject == child; });
	}
	else {
		std::vector<SceneObject*> subChildren = sceneObject->getChildren();
		std::remove_if(children.begin(), children.end(), [sceneObject](SceneObject * child) { return sceneObject == child; });
		for(auto subChild : subChildren) {
			children.push_back(subChild);
		}
	}

	sceneObject->dispose();
	delete sceneObject;
}

std::string SceneObject::getId()
{
	return id;
}

void SceneObject::setId(std::string id)
{
	this->id = id;
}

void SceneObject::dispose() {
	for(int i = 0; i < children.size(); i++) {
		children[i]->dispose();
		delete children[i];
	}
	children.clear();
}

Transform SceneObject::getWorldTransform() {
	return cachedWorldTransform;
}

void SceneObject::transformChanged() {
	cachedWorldTransform = transform * parentTransform;
	parentTransformChanged(cachedWorldTransform);	
}

void SceneObject::parentTransformChanged(Transform transform) {
	for(auto * child : children) { 
		child->parentTransform = transform;
		child->cachedWorldTransform = child->transform * transform;
		child->parentTransformChanged(child->cachedWorldTransform);		
	}
	updateDirection(this->transform);
}