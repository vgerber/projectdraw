#include "car.h"

Car::Car()
{
	drawable.setModel(primitives::generateQuad(1.0f, 1.0f, 2.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	drawable.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	rigidBody = RigidBody(collision::CollisionShape(collision::generateCube(drawable.getSize())), drawable.getPositionCenter(), drawable.getRotation(), 1000.0f);
	rigidBody.setDrawable(drawable);
}

void Car::addToScene(Scene & scene)
{
	scene.addDrawable(drawable);
	scene.addRigidBody(rigidBody, 2, 3);

	//only in one scene
	vehicle = new Vehicle(&rigidBody, scene.getPhysicsWorld());
	scene.addVehicle(*vehicle);

	glm::vec3 connectionPoins[4] = {
		glm::vec3(0.5f, -0.5f, 0.7f),
		glm::vec3(-0.5f, -0.5f, 0.7f),
		glm::vec3(0.5f, -0.5f, -0.7f),
		glm::vec3(-0.5f, -0.5f, -0.7f),
	};
	for (int i = 0; i < 4; i++) {
		Drawable *wheelD = new Drawable();
		wheelD->setModel(primitives::generateQuad(0.1f, 0.1f, 0.1f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)));
		wheels.push_back(wheelD);
		Wheel wheel;
		wheel.drawable = wheels[i];
		vehicle->addWheel(wheel, connectionPoins[i], i < 2);
	}
}

void Car::steer(float degree)
{

}

void Car::move(float strength)
{
	for (int i = 0; i < 4; i++) {
		vehicle->getVehicle()->applyEngineForce(strength, i);
	}
}
