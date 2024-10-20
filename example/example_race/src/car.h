#pragma once

#include "Core/common.h"
#include "Core/Util/vehicle.h"

class Car {
public:
	Car();

	void addToScene(Scene &scene);

	void steer(float degree);
	void move(float strength);

private:
	Drawable drawable;
	std::vector<Drawable*> wheels;
	RigidBody rigidBody;
	Vehicle *vehicle;

};