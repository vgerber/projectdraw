#include "vehicle.h"

Vehicle::Vehicle(RigidBody *chassis, btDiscreteDynamicsWorld *world)
{
	chassis->getBody()->setLinearFactor(btVector3(1.0f, 1.0f, 1.0f));
	chassis->getBody()->setAngularFactor(btVector3(1.0f, 1.0f, 1.0f));
	this->rigidBody = chassis;
	this->rigidBody->getBody()->setActivationState(DISABLE_DEACTIVATION);
	this->rigidBody->visibleAABB = true;

	vehicleRaycaster = new btDefaultVehicleRaycaster(world);
	btVehicle = new btRaycastVehicle(tuning, chassis->getBody(), vehicleRaycaster);
}

void Vehicle::addWheel(Wheel wheel, glm::vec3 connectionPoint, bool isFrontWheel)
{
	btVector3 wheelDirectionCS0(0, -1, 0);
	//rotating axis
	btVector3 wheelAxleCS(1, 0, 0);
	btScalar suspensionRestLength(wheel.suspensionRestLength);
	btScalar wheelWidth(wheel.drawable->getSize().width);
	btScalar wheelRadius(wheel.drawable->getSize().depth * 0.5);

	btVehicle->addWheel(toBtVec3(connectionPoint), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, isFrontWheel);
	wheels.push_back(wheel);

	btWheelInfo &wheelInfo = btVehicle->getWheelInfo(wheels.size()-1);
	wheelInfo.m_suspensionStiffness = wheel.suspensionStiffness;
	wheelInfo.m_wheelsDampingCompression = wheel.dampingCompression * 2 * btSqrt(wheelInfo.m_suspensionStiffness);
	wheelInfo.m_wheelsDampingRelaxation = wheel.dampingRelaxation * 2 * btSqrt(wheelInfo.m_suspensionStiffness);
	wheelInfo.m_frictionSlip = wheel.frictionSlip;
	wheelInfo.m_rollInfluence = wheel.rollInfluence;
}

void Vehicle::sync()
{
	rigidBody->syncDrawable();
	glm::vec4 carDirection = glm::vec4(toVec3(btVehicle->getForwardVector().cross(btVector3(1.0f, 0.0f, 0.0f))), 0.0f);
	direction = glm::normalize(rigidBody->getDrawable()->getModelMatrix() * carDirection);

	btTransform transform;

	//get length of movement
	glm::vec3 pos = rigidBody->getDrawable()->getPositionCenter() - position;

	//std::cout << "Position Vehicle (" << position.x << "," << position.y << "," << position.z << ")" << std::endl;

	float length = sqrtf(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
	// get driving direction (back or front)
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

	position = rigidBody->getDrawable()->getPositionCenter();
}

void Vehicle::draw()
{
	rigidBody->getDrawable()->draw();
	for (auto wheel : wheels) {
		wheel.drawable->draw();
	}
}

void Vehicle::dispose()
{
	delete btVehicle;
	delete vehicleRaycaster;
}

glm::vec3 Vehicle::getFront()
{
	return direction;
}

btRaycastVehicle * Vehicle::getVehicle()
{
	return btVehicle;
}

RigidBody * Vehicle::getChassis()
{
	return rigidBody;
}
