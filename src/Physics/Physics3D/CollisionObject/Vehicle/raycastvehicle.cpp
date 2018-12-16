#include "raycastvehicle.h"

RaycastVehicle::RaycastVehicle(collision::CollisionShape chassisShape, float mass, DiscreteWorld &world) : RigidBody(chassisShape, mass, RigidType::DYNAMIC) {
    raycastVehcileRaycaster = new btDefaultVehicleRaycaster(static_cast<btDiscreteDynamicsWorld*>(world.getWorldHandle()));
    raycastVehicleTuning = new btRaycastVehicle::btVehicleTuning();
    raycastVehicle = new btRaycastVehicle(*raycastVehicleTuning, rigidBody, raycastVehcileRaycaster);
}

void RaycastVehicle::addWheel(RaycastVehicleWheel wheel) {
    raycastVehicle->addWheel(
        toBtVec3(wheel.contactPoint),
        toBtVec3(wheel.rayDirection),
        toBtVec3(wheel.rotatingAxel),
        wheel.suspensionRestLength,
        wheel.radius,
        *raycastVehicleTuning,
        wheel.canSteer
    );

    wheels.push_back(wheel);
    updateWheel(wheels.size()-1, wheel);
}

RaycastVehicleWheel RaycastVehicle::getWheel(int index) {
    return wheels[index];
}

void RaycastVehicle::updateWheel(int index, RaycastVehicleWheel wheel) {
    wheels[index] = wheel;
    btWheelInfo &wheelInfo = raycastVehicle->getWheelInfo(index);
    wheelInfo.m_bIsFrontWheel = wheel.canSteer;
    
    wheelInfo.m_wheelDirectionCS = toBtVec3(wheel.rayDirection);
    wheelInfo.m_chassisConnectionPointCS = toBtVec3(wheel.contactPoint);
    wheelInfo.m_wheelAxleCS = toBtVec3(wheel.rotatingAxel);

    wheelInfo.m_suspensionStiffness = wheel.suspensionStiffness;
    wheelInfo.m_wheelsDampingCompression = wheel.dampingCompression;
    wheelInfo.m_wheelsDampingRelaxation = wheel.dampingRelaxation;
    wheelInfo.m_frictionSlip = wheel.dampingRelaxation;
    wheelInfo.m_rollInfluence = wheel.rollInfluence;
}

void RaycastVehicle::move(float forward) {
    float force = maxForwardForce * forward;
    for(int i = 0; i < wheels.size(); i++) {
        if(wheels[i].canAccelerate) {
            raycastVehicle->applyEngineForce(force, i);
        }
    }    
}

void RaycastVehicle::steer(float right) {
    float angle = right;
    for(int i = 0; i < wheels.size(); i++) {
        if(wheels[i].canSteer) {
            raycastVehicle->setSteeringValue(right, i);
        }
    }
}

void RaycastVehicle::brake(float brake) {
    float force = maxBrakeForce * brake;
    for(int i = 0; i < wheels.size(); i++) {
        if(wheels[i].canBrake) {
            raycastVehicle->setBrake(force, i);
        }
    }
}

void RaycastVehicle::setMaxForwardForce(float force) {
    maxForwardForce = force;
} 

void RaycastVehicle::setMaxBrakeForce(float force) {
    maxBrakeForce = force;
}

void RaycastVehicle::setMaxSteeringAngle(float radians) {
    maxSteeringAngle = radians;
}

void RaycastVehicle::update() {
    btTransform chassisTransform = raycastVehicle->getChassisWorldTransform();
    btQuaternion chassisRotation = chassisTransform.getRotation();
    Rotator chassisRotator;
    chassisRotator.rotateAxis(chassisRotation.getAngle(), toGLMVec3(chassisRotation.getAxis()));
    if(drawable) {        
        drawable->setPosition(toGLMVec3(chassisTransform.getOrigin()));
        drawable->rotate(chassisRotator);
    }

    for(int i = 0; i < wheels.size(); i++) {
        RaycastVehicleWheel wheel = wheels[i];
        if(wheel.drawable) {
            btTransform wheelTransform = raycastVehicle->getWheelTransformWS(i);
            btQuaternion wheelRotation = wheelTransform.getRotation();
            float steering = raycastVehicle->getSteeringValue(i);
            Rotator wheelRotator;
            Rotator wheelSteerRotator;
            //wheelRotator.rotateAxis(wheelRotation.getAngle(), toGLMVec3(wheelRotation.getAxis()));
            
            wheelSteerRotator.rotateAxis(maxSteeringAngle *  steering, glm::vec3(0.0, 0.0, 1.0));
            //wheelRotator.rotateAxis(, glm::vec3(1.0, 0.0, 0.0));
            wheelRotator.applyRotation(wheelSteerRotator);

            wheel.drawable->setPosition(toGLMVec3(wheelTransform.getOrigin()));
            wheelRotator.applyRotation(chassisRotator);
            wheel.drawable->rotate(wheelRotator);            
        }
    }
}

btRaycastVehicle * RaycastVehicle::getVehicleObjectHandle() {
    return raycastVehicle;
}
