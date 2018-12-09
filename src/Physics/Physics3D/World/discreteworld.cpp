#include "discreteworld.h"

DiscreteWorld::DiscreteWorld() {
    setup();
}

void DiscreteWorld::addPhysicsObject(PhyscisObject &physicsObject) {
    if(RigidBody * rBody = dynamic_cast<RigidBody*>(&physicsObject)) {
        rigidBodies.push_back(rBody);
        world->addRigidBody(rBody->getBody());
        return;
    }
    if(TriggerVolume * tVolume = dynamic_cast<TriggerVolume*>(&physicsObject)) {
        triggerVolumes.push_back(tVolume);
        return;
    }
    printf("Discrete World does not support this type\n");
}

void DiscreteWorld::removePhysicsObject(PhyscisObject &physicsObject) {
    if(RigidBody * rBody = dynamic_cast<RigidBody*>(&physicsObject)) {
        for(size_t i = 0; i < rigidBodies.size(); i++) {
            if(rigidBodies[i] == rBody) {
                rigidBodies.erase(rigidBodies.begin() + i);
                i--;
            }
        }
        return;
    }
    if(TriggerVolume * tVolume = dynamic_cast<TriggerVolume*>(&physicsObject)) {
        for(size_t i = 0; i < triggerVolumes.size(); i++) {
            if(triggerVolumes[i] == tVolume) {
                triggerVolumes.erase(triggerVolumes.begin() + i);
                i--;
            }
        }
        return;
    }
}

void DiscreteWorld::update(float delta) {
    world->stepSimulation(delta, maxSimulationSteps, fixedSimulationStep);
    
    for(auto rBody : rigidBodies) {
        rBody->update();
    }
}

void DiscreteWorld::setQuality(int maxSteps, float fixedTimeStep) {
    maxSimulationSteps = maxSteps;
    fixedSimulationStep = fixedTimeStep;
}


void DiscreteWorld::setup() {
    collisionConfig = new btDefaultCollisionConfiguration();
    collisionDispatcher = new btCollisionDispatcher(collisionConfig);
    broadphase = new btDbvtBroadphase();
    constrainSolver = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(collisionDispatcher, broadphase, constrainSolver, collisionConfig);
    world->setGravity(btVector3(0.0f, 0.0f, -9.81f));
}

