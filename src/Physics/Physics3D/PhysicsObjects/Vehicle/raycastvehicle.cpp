#include "raycastvehicle.h"

void RaycastVehicle::move(float forward) {
    float force = maxForwardForce * forward;
}

void RaycastVehicle::steer(float right) {
    float angle = maxSteeringAngle * right;
}

void RaycastVehicle::brake(float brake) {
    float force = maxBrakeForce * brake;
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
    
}
