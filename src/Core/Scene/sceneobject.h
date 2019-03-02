#pragma once

#include <string>

#include "Core/Util/Transform/moveable.h"

class SceneObject : public Moveable
{
public:
	SceneObject();
	~SceneObject();

	SceneObject * getChild(std::string id);
	void addChild(SceneObject * sceneObject);

	std::vector<SceneObject*> getChildren();

	std::string getId();

	///free any allocated memory
	///destructor would cause issues when object is shared
	virtual void dispose();

	void setId(std::string id);
	
	///get local transform combined with parent transform
	Transform getWorldTransform();

protected:
	std::string id;

	std::vector<SceneObject*> children;

	///Transform from root to parent
	Transform parentTransform;

	///notifies all children when parents transform has been changed
	virtual void transformChanged() override;

	///update cached modelTranform
	virtual void parentTransformChanged(Transform transform);
private:
	///Cached transform with parent, base and model transform
	///Refreshed in each transformChnaged() and parenTransformChanged()
	Transform cachedWorldTransform;

};

