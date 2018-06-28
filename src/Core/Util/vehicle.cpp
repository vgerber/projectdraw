#include "vehicle.h"

Vehicle::Vehicle(RigidBody *rigidBody, btDiscreteDynamicsWorld *world)
{

	this->rigidBody = rigidBody;
	this->rigidBody->getBody()->setActivationState(DISABLE_DEACTIVATION);
	this->rigidBody->visibleAABB = true;

	vehicleRaycaster = new btDefaultVehicleRaycaster(world);

	btVehicle = new btRaycastVehicle(tuning, rigidBody->getBody(), vehicleRaycaster);
}

void Vehicle::setChassis(Drawable * chassis)
{
	this->chassis = chassis;
}

void Vehicle::addWheel(Wheel wheel, glm::vec3 connectionPoint, bool isFrontWheel)
{
	btVector3 wheelDirectionCS0(0, -1, 0);
	//rotating axis
	btVector3 wheelAxleCS(1, 0, 0);
	btScalar suspensionRestLength(1.3f);
	btScalar wheelWidth(wheel.drawable->getSize().width);
	btScalar wheelRadius(wheel.drawable->getSize().depth * 0.5);

	btVehicle->addWheel(toBtVec3(connectionPoint), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, isFrontWheel);
	wheels.push_back(wheel);

	btWheelInfo &wheelInfo = btVehicle->getWheelInfo(wheels.size()-1);
	wheelInfo.m_suspensionStiffness = 100;
	wheelInfo.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheelInfo.m_suspensionStiffness);
	wheelInfo.m_wheelsDampingRelaxation = btScalar(0.3) * 2 * btSqrt(wheelInfo.m_suspensionStiffness);

	wheelInfo.m_frictionSlip = btScalar(1.2);
	wheelInfo.m_rollInfluence = 1;
}

void Vehicle::sync()
{
	if (chassis) {
		chassis->transform(rigidBody->getBody()->getWorldTransform());
	}
	glm::vec4 carDirection = glm::vec4(toVec3(btVehicle->getForwardVector().cross(btVector3(1.0f, 0.0f, 0.0f))), 0.0f);
	direction = glm::normalize(chassis->getModelMatrix() * carDirection);

	btTransform transform;

	//get length of movement
	glm::vec3 pos = chassis->getPositionCenter() - position;
	float length = sqrtf(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
	// get driving direction (back or forward)
	length *= (glm::dot(pos, direction) >= 0.0f ? 1.0f : -1.0f);


	for (int i = 0; i < wheels.size(); i++) {
		Wheel wheel = wheels[i];
		if (wheel.drawable) {	
			transform = btVehicle->getWheelTransformWS(i);
			btQuaternion rot = transform.getRotation();



			btScalar steering = glm::radians(90.0f * btVehicle->getSteeringValue(i));

			//calculate velocity for each wheel
			btScalar velocity = wheel.wheelRotation + glm::radians((30.0f * -length) / wheel.drawable->getSize().depth);
			wheel.wheelRotation = velocity;

			//set wheel roation (steering and velocity)
			btQuaternion wheelRotation(steering, velocity, 0.0f);
			transform.setRotation(rigidBody->getBody()->getWorldTransform().getRotation() * wheelRotation);
			wheel.drawable->transform(transform);
			wheel.wheelRotation = velocity;

			wheels[i] = wheel;
		}
	}

	position = chassis->getPositionCenter();	
}

void Vehicle::draw()
{
	chassis->draw();
	for (auto wheel : wheels) {
		wheel.drawable->draw();
	}
}

void Vehicle::dispose()
{
	delete btVehicle;
	delete vehicleRaycaster;
}

glm::vec3 Vehicle::getDirection()
{
	return direction;
}

btRaycastVehicle * Vehicle::getVehicle()
{
	return btVehicle;
}
