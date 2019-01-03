#include "collisionobject.h"

void CollisionObject::update() {
}

btCollisionObject * CollisionObject::getCollisionObjectHandle() {
    return collisionObject;
}

Transform CollisionObject::getWorldTransform() {
    btTransform bodyTransform = collisionObject->getWorldTransform();
    btQuaternion bodyRotation = bodyTransform.getRotation();
    Transform newTransform;
    newTransform.translate(toGLMVec3(bodyTransform.getOrigin()));
    newTransform.rotate(Rotator(glm::quat(bodyRotation.x(), bodyRotation.y(), bodyRotation.z(), bodyRotation.w()), glm::vec3(0.0)));
    return newTransform;
}

void CollisionObject::setUserPointer(void * userPtr) {
    this->userPtr = userPtr;
}

void * CollisionObject::getUserPointer() const {
    return userPtr;
}