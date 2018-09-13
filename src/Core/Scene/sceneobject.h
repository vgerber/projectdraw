#pragma once

#include <string>

#include "Util/moveable.h"

class SceneObject : public Moveable
{
public:
	SceneObject();
	~SceneObject();

	virtual glm::vec3 getPosition();
	std::string getId();

	virtual void setPosition(glm::vec3 position);
	virtual void dispose() = 0;
	void setId(std::string id);


protected:
	std::string id;
};

