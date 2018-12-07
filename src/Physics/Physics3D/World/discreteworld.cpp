#include "discreteworld.h"

DiscreteWorld::DiscreteWorld() {
    setup();
}


void DiscreteWorld::setup() {
    collisionConfig = new btDefaultCollisionConfiguration();
    collisionDispatcher = new btCollisionDispatcher(collisionConfig);
    broadphase = new btDbvtBroadphase();
    constrainSolver = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(collisionDispatcher, broadphase, constrainSolver, collisionConfig);
}

