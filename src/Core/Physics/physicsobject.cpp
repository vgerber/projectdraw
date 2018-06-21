#include "physicsobject.h"

RigidBody::RigidBody() {
    rigidBody = nullptr;
    shape = nullptr;
}

RigidBody::RigidBody(collision::CollisionShape shape, glm::vec3 center, glm::vec3 rotation, GLfloat mass) {
        this->shape = new collision::CollisionShape(shape);
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(center.x, center.y, center.z));
		transform.setRotation(btQuaternion(rotation.y, rotation.x, rotation.z));
		
		btScalar bt_mass(mass);

		bool isDynamic = (bt_mass != 0.0f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic) {
			this->shape->get_shape()->calculateLocalInertia(bt_mass, localInertia);
		}

		btDefaultMotionState *myMotionState = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, this->shape->get_shape(), localInertia);
		rigidBody = new btRigidBody(rbInfo);	    
}

void RigidBody::setDrawable(Drawable &drawable)
{
	this->drawable = &drawable;
}

void RigidBody::syncBody()
{
	glm::vec3 center = drawable->getPositionCenter();
	glm::vec3 rotation = drawable->getRotation();
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(center.x, center.y, center.z));
	transform.setRotation(btQuaternion(rotation.y, rotation.x, rotation.z));
	rigidBody->setWorldTransform(transform);
	
	/*
	btTransform transform;
	transform.setFromOpenGLMatrix(glm::value_ptr(drawable->getModelMatrix()));
	rigidBody->setWorldTransform(transform);
	*/
}

void RigidBody::syncDrawable()
{
	btTransform transform;
	if (rigidBody && rigidBody->getMotionState()) {
		rigidBody->getMotionState()->getWorldTransform(transform);
	}
	else {
		transform = rigidBody->getWorldTransform();
	}
	drawable->transform(transform);
}

void RigidBody::dispose()
{
	delete rigidBody->getMotionState();
	delete rigidBody;
	delete shape;
}

btRigidBody* RigidBody::getBody() {
    return rigidBody;
}

Drawable * RigidBody::getDrawable()
{
	return drawable;
}



uint PhysicsObject::addRigidBody(RigidBody rigidBody) {
    rigidBodys[id_counter] = rigidBody;
    return id_counter++;
}

RigidBody PhysicsObject::getRigidBody(uint id) {
    return rigidBodys[id];
}

std::map<uint, class RigidBody> PhysicsObject::getRigidBodys() {
    return rigidBodys;
}