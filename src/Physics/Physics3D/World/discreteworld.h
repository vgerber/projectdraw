#pragma once

#include <vector>

#include "../PhysicsObjects/Rigidbody/rigidbody.h"
#include "../PhysicsObjects/Trigger/triggervolume.h"

#include "../Util/debugdrawer.h"
#include "LinearMath/btIDebugDraw.h"

class DiscreteWorld {
public:
    DiscreteWorld();

    void addPhysicsObject(PhyscisObject &pobject);
    void removePhysicsObject(PhyscisObject &pobject);

    void update(float delta);

    void setQuality(int maxSteps, float fixedTimeStep);

    //do not manipulate them outside!
    std::vector<Drawable*> getDebugDrawables();

private:
    DebugDrawer * debugDrawer;
    int maxSimulationSteps = 10;
    float fixedSimulationStep = 1.0f / 60.0f;
    std::vector<RigidBody*> rigidBodies;
    std::vector<TriggerVolume*> triggerVolumes;

    btDefaultCollisionConfiguration * collisionConfig = nullptr;
    btCollisionDispatcher * collisionDispatcher = nullptr;
    btDbvtBroadphase * broadphase = nullptr;
    btSequentialImpulseConstraintSolver * constrainSolver = nullptr;
    btDiscreteDynamicsWorld * world = nullptr;

    void setup();
};