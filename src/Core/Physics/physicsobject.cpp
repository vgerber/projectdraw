#include "physicsobject.h"

RigidBody::RigidBody() {
    rigid_body = nullptr;
    shape = nullptr;
}

RigidBody::RigidBody(collision::CollisionShape *shape, glm::vec3 center, glm::vec3 rotation, GLfloat mass) {
        this->shape = shape;
		btTransform ground_transform;
		ground_transform.setIdentity();
		ground_transform.setOrigin(btVector3(center.x, center.y, center.z));
		ground_transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
		
		btScalar bt_mass(mass);

		bool isDynamic = (bt_mass != 0.0f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic) {
			this->shape->get_shape()->calculateLocalInertia(bt_mass, localInertia);
		}

		btDefaultMotionState *myMotionState = new btDefaultMotionState(ground_transform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, this->shape->get_shape(), localInertia);
		rigid_body = new btRigidBody(rbInfo);	        
}

btRigidBody* RigidBody::get_body() {
    return rigid_body;
}



uint PhysicsObject::add_rigid_body(RigidBody rigid_body) {
    rigid_bodys[id_counter] = rigid_body;
    return id_counter++;
}

RigidBody PhysicsObject::get_rigid_body(uint id) {
    return rigid_bodys[id];
}

std::map<uint, class RigidBody> PhysicsObject::get_rigid_bodys() {
    return rigid_bodys;
}