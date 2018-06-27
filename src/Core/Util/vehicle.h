#pragma once

#include "../core.h"
#include "../Scene/drawable.h"
#include "../Physics/physics.h"

struct Wheel {
	Drawable *drawable = nullptr;
	float wheelRotation = 0.0f;
};


class Vehicle {
public:
	Vehicle(RigidBody *rigidBody, btDiscreteDynamicsWorld *world);

	void setChassis(Drawable *chassis);
	void addWheel(Wheel wheel, glm::vec3 connectionPoint, bool isFrontWheel);

	void sync();
	void draw();

	void dispose();

	glm::vec3 getDirection();

	btRaycastVehicle *getVehicle();

private:
	btRaycastVehicle *btVehicle = nullptr;
	btVehicleRaycaster *vehicleRaycaster = nullptr;
	btRaycastVehicle::btVehicleTuning tuning;
	RigidBody *rigidBody = nullptr;
	std::vector<Wheel> wheels;
	Drawable* chassis = nullptr;
	glm::vec3 direction;
	glm::vec3 position;

};