#pragma once

#include <bullet/BulletDynamics/Vehicle/btRaycastVehicle.h>

#include "Physics/Physics3D/CollisionObject/Rigidbody/rigidbody.h"
#include "Physics/Physics3D/World/discreteworld.h"

#include <vector>

class DiscreteWorld;

struct RaycastVehicleWheel {
    Drawable *drawable = nullptr;
    
    //true if wheel can be used for steering
    bool canSteer = false;
    //true if wheel can reduce velocity of vehicle
    bool canBrake = false;
    //true if wheel can apply force to vehicle
    bool canAccelerate = false;

    glm::vec3 rotatingAxel = glm::vec3(1.0, 0.0, 0.0);
    glm::vec3 rayDirection = glm::vec3(0.0, 0.0, -1.0);
    glm::vec3 contactPoint = glm::vec3(0.0);

    float width = 1.0;
    float radius = 0.2;

    float suspensionRestLength = 0.5f;
	float suspensionStiffness = 100.0f;
	float dampingCompression = 0.2f;
	float dampingRelaxation = 0.3f;
	float frictionSlip = 10.0f;
	float rollInfluence = 1.0f;  


    float velocityRotation = 0.0f;
};

class RaycastVehicle : public RigidBody {
public:
    RaycastVehicle(collision::CollisionShape chassisShape, float mass, DiscreteWorld &world);

    void addWheel(RaycastVehicleWheel wheel);

    RaycastVehicleWheel getWheel(int index);
    void updateWheel(int index, RaycastVehicleWheel wheel);

    virtual void move(float forward  = 1.0f);
    virtual void steer(float right = 1.0f);
    virtual void brake(float brake = 1.0);

    void setMaxForwardForce(float force);
    void setMaxBrakeForce(float force);
    void setMaxSteeringAngle(float radians);

    virtual void update() override;

    btRaycastVehicle * getVehicleObjectHandle();

private:
    float maxForwardForce = 0.0f;
    float maxBrakeForce = 0.0f;
    float maxSteeringAngle = 0.0f;

    btRaycastVehicle::btVehicleTuning * raycastVehicleTuning = nullptr;
    btVehicleRaycaster * raycastVehcileRaycaster = nullptr;
    btRaycastVehicle * raycastVehicle = nullptr; 

    std::vector<RaycastVehicleWheel> wheels;   
};