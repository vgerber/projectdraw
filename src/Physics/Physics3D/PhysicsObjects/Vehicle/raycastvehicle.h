#pragma once


class RaycastVehicle {
public:

    virtual void move(float forward  = 1.0f);
    virtual void steer(float right = 1.0f);
    virtual void brake(float force = 1.0);

    void setMaxForwardForce(float force);
    void setMaxBrakeForce(float force);
    void setMaxSteeringAngle(float radians);

private:
    float maxForwardForce = 0.0f;
    float maxBrakeForce = 0.0f;
    float maxSteeringAngle = 0.0f;
};