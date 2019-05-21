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
	callUpdateTree();
}

std::vector<SceneObject*> SceneObject::getChildren() const {
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
		children.erase(std::remove_if(children.begin(), children.end(), [sceneObject](SceneObject * child) { return sceneObject == child; }), children.end());
	}
	else {
		std::vector<SceneObject*> subChildren = sceneObject->getChildren();
		children.erase(std::remove_if(children.begin(), children.end(), [sceneObject](SceneObject * child) { return sceneObject == child; }), children.end());
		for(auto subChild : subChildren) {
			children.push_back(subChild);
		}
	}

	sceneObject->dispose();
	delete sceneObject;
	callUpdateTree();
}

std::string SceneObject::getId() const
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

Transform SceneObject::getWorldTransform() const {
	return cachedWorldTransform;
}

void SceneObject::beginEdit() {
	editMode = true;
}

void SceneObject::endEdit() {
	editMode = false;
	callUpdate();
	callUpdateTree();
}

void SceneObject::addUpdateListener(void * receiver, std::function<void()> receiverFunction) {
	if(receiver) {
		updateListeners.push_back(std::make_pair(receiver, receiverFunction));
	}
}

void SceneObject::removeUpdateListener(void * receiver) {
	for(int i = 0; i < updateListeners.size(); i++) {
		if(receiver == updateListeners[i].first) {
			updateListeners.erase(updateListeners.begin() + i);
			i--;
		}
	}
}

void SceneObject::addUpdateTreeListener(void * receiver, std::function<void(SceneObject*)> receiverFunction) {
	if(receiver) {
		updateTreeListeners.push_back(std::make_pair(receiver, receiverFunction));
	}
}

void SceneObject::removeUpdateTreeListener(void * receiver) {
	updateTreeListeners.erase(
		std::remove_if(
			updateTreeListeners.begin(),
			updateTreeListeners.end(),
			[receiver](auto listener) { return receiver == listener.first; }
		),
		updateTreeListeners.end()
	);
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

void SceneObject::callUpdate() {
	if(!editMode) {
		for(auto listener : updateListeners) {
			listener.second();
		}
	}
}

void SceneObject::callUpdateTree() {
	if(!editMode) {
		for(auto listener : updateTreeListeners) {
			listener.second(this);
		}
	}
}