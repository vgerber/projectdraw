#include "discreteworld.h"

DiscreteWorld::DiscreteWorld() {
    setup();
}

void DiscreteWorld::addCollisionObject(CollisionObject &collisionObject) {
	if (RaycastVehicle * vehicle = dynamic_cast<RaycastVehicle*>(&collisionObject)) {
		raycastVehicles.push_back(vehicle);
		static_cast<btDynamicsWorld*>(world)->addRigidBody(static_cast<btRigidBody*>(vehicle->getCollisionObjectHandle()));
		static_cast<btDynamicsWorld*>(world)->addVehicle(vehicle->getVehicleObjectHandle());
	}
    else if(RigidBody * rBody = dynamic_cast<RigidBody*>(&collisionObject)) {
        rigidBodies.push_back(rBody);
        static_cast<btDynamicsWorld*>(world)->addRigidBody(static_cast<btRigidBody*>(rBody->getCollisionObjectHandle()));
    }
    else if(TriggerVolume * tVolume = dynamic_cast<TriggerVolume*>(&collisionObject)) {
        triggerVolumes.push_back(tVolume);
        world->addCollisionObject(tVolume->getCollisionObjectHandle());
    }
    collisionObjects.push_back(&collisionObject);
}

void DiscreteWorld::removeCollisionObject(CollisionObject &collisionObject) {
    if(RigidBody * rBody = dynamic_cast<RigidBody*>(&collisionObject)) {
        for(size_t i = 0; i < rigidBodies.size(); i++) {
            if(rigidBodies[i] == rBody) {
                rigidBodies.erase(rigidBodies.begin() + i);
                i--;
            }
        }
        return;
    }
    if(TriggerVolume * tVolume = dynamic_cast<TriggerVolume*>(&collisionObject)) {
        for(size_t i = 0; i < triggerVolumes.size(); i++) {
            if(triggerVolumes[i] == tVolume) {
                triggerVolumes.erase(triggerVolumes.begin() + i);
                i--;
            }
        }
        return;
    }
    for(int i = 0; i < collisionObjects.size(); i++) {
        if(collisionObjects[i] == &collisionObject) {
            collisionObjects.erase(collisionObjects.begin() + i);
            i--;
        }
    }
}

void DiscreteWorld::update(float delta) {
    static_cast<btDiscreteDynamicsWorld*>(world)->stepSimulation(delta, maxSimulationSteps, fixedSimulationStep);
    
    for(auto collisionObject : collisionObjects) {
        collisionObject->update();
    }

    int numMainifolds = world->getDispatcher()->getNumManifolds();
    for(int i = 0; i < numMainifolds; i++) {
        btPersistentManifold * contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
        CollisionObject * objectA = static_cast<CollisionObject*>(contactManifold->getBody0()->getUserPointer());
        CollisionObject * objectB = static_cast<CollisionObject*>(contactManifold->getBody1()->getUserPointer());
        if(objectA->onCollide) {
            objectA->onCollide(objectB);
        }
        if(objectB->onCollide) {
            objectB->onCollide(objectA);
        }
    } 

	world->debugDrawWorld();
}

void DiscreteWorld::setQuality(int maxSteps, float fixedTimeStep) {
    maxSimulationSteps = maxSteps;
    fixedSimulationStep = fixedTimeStep;
}

void DiscreteWorld::setup() {
    collisionConfig = new btDefaultCollisionConfiguration();
    collisionDispatcher = new btCollisionDispatcher(collisionConfig);
    broadphase = new btDbvtBroadphase();
    broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
    constrainSolver = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(collisionDispatcher, broadphase, constrainSolver, collisionConfig);
    static_cast<btDynamicsWorld*>(world)->setGravity(btVector3(0.0f, 0.0f, -10.0f));
    world->setDebugDrawer(&debugDrawer);

    debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb);
    
}

