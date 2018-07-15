#pragma once

#include "../core.h"
#include "../Scene/drawable.h"
#include "../Physics/physics.h"

struct Wheel {
	Drawable *drawable = nullptr;
	float wheelRotation = 0.0f;

	float suspensionRestLength = 0.5f;
	float suspensionStiffness = 100.0f;
	float dampingCompression = 0.2f;
	float dampingRelaxation = 0.3f;
	float frictionSlip = 10.0f;
	float rollInfluence = 1.0f;
};


class Vehicle {
public:
	Vehicle(RigidBody *chassis, btDiscreteDynamicsWorld *world);

	void addWheel(Wheel wheel, glm::vec3 connectionPoint, bool isFrontWheel);

	void sync();
	void draw();

	void dispose();

	glm::vec3 getFront();

	btRaycastVehicle *getVehicle();

	RigidBody *getChassis();

private:
	btRaycastVehicle *btVehicle = nullptr;
	btVehicleRaycaster *vehicleRaycaster = nullptr;
	btRaycastVehicle::btVehicleTuning tuning;
	RigidBody *rigidBody = nullptr;
	std::vector<Wheel> wheels;
	glm::vec3 direction;
	glm::vec3 position;

};