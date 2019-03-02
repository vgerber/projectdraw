#pragma once

#include <vector>
#include <functional>

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "Core/Util/Transform/moveable.h"
#include "Physics/Physics3D/Util/conversion.h"


class CollisionObject : public Moveable {
public:
    std::function<void(CollisionObject*)> onCollide;

    virtual void update();

    virtual btCollisionObject * getCollisionObjectHandle();

    virtual Transform getWorldTransform();

    void setUserPointer(void * userPtr);
    void * getUserPointer() const;

protected:
    void * userPtr = nullptr;
    btCollisionObject * collisionObject = nullptr;
};

struct CollisionResult {
    const CollisionObject * objectA;
    const CollisionObject * objectB;
    std::vector<glm::vec3> collisionPointsA;
    std::vector<glm::vec3> collisionPointsB;
};