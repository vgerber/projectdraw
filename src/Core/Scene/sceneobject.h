#pragma once

#include <string>

#include "Util/moveable.h"

class SceneObject : public Moveable
{
public:
	SceneObject();
	~SceneObject();

	std::string getId();

	virtual void dispose() = 0;
	void setId(std::string id);


protected:
	std::string id;
};

