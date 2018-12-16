#pragma once

#include <vector>
#include <functional>

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>


class CollisionObject {
public:
    std::function<void(const CollisionObject*)> onCollide;

    virtual void update() = 0;

    virtual btCollisionObject * getCollisionObjectHandle() = 0;

protected:
};

struct CollisionResult {
    const CollisionObject * objectA;
    const CollisionObject * objectB;
    std::vector<glm::vec3> collisionPointsA;
    std::vector<glm::vec3> collisionPointsB;
};