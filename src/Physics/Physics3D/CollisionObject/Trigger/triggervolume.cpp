#include "triggervolume.h"


TriggerVolume::TriggerVolume(collision::CollisionShape shape, glm::vec3 position) {
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(toBtVec3(position));
    ghostObject = new btGhostObject();
    ghostObject->setCollisionShape(shape.getShape());
    ghostObject->setWorldTransform(transform);
    ghostObject->setUserPointer(this);
    ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void TriggerVolume::update() {
    //printf("%d\n", ghostObject->getNumOverlappingObjects());
}

std::vector<CollisionResult> TriggerVolume::getCollisions() {
    std::vector<CollisionResult> collisions;

    for(int i = 0; i < ghostObject->getNumOverlappingObjects(); i++) {
        CollisionResult collision;
        collision.objectA = this;
        collision.objectB = static_cast<CollisionObject*>(ghostObject->getOverlappingObject(i)->getUserPointer());
    }

    return collisions;
}

btCollisionObject * TriggerVolume::getCollisionObjectHandle() {
    return static_cast<btCollisionObject*>(ghostObject);
}

void TriggerVolume::updateModel() {
    btTransform transform;
    transform.setOrigin(toBtVec3(position));
    glm::vec3 rotationEuler = rotator.getRotationEuler();
    btQuaternion quaternion(rotationEuler.x, rotationEuler.y, rotationEuler.z);
    transform.setRotation(quaternion);
    ghostObject->setWorldTransform(transform);
}

