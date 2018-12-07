#pragma once

#include <vector>

#include "../PhysicsObjects/physicsobject.h"

class DiscreteWorld {
public:
    DiscreteWorld();

    void addPhysicsObject(PhyscisObject &pobject);
    void removePhysicsObject(PhyscisObject &pobject);

    void update(float delta);
    void refreshDrawables();

private:
    std::vector<PhyscisObject*> physcisObjects;

    btDefaultCollisionConfiguration * collisionConfig = nullptr;
    btCollisionDispatcher * collisionDispatcher = nullptr;
    btDbvtBroadphase * broadphase = nullptr;
    btSequentialImpulseConstraintSolver * constrainSolver = nullptr;
    btDiscreteDynamicsWorld * world = nullptr;

    void setup();
};