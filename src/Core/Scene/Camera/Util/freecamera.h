#pragma once

#include "Core/Scene/Camera/perspcamera.h"

class FreeCamera : public PerspectiveCamera {
public:
    void moveUp(float value);
    void moveRight(float value);
    void moveForward(float value);
    void tiltUp(float value);
    void tiltRight(float value);

    void setMoveSpeed(float speed);
    void setTiltSpeed(float speed);
    void setMaxPitch(float radians);

private:
    float maxUpAngle = glm::radians(90.0f);
    float moveSpeed = 1.0f;
    float tiltSpeed = 1.0f;
    float pitch = 0.0f, yaw= 0.0f;
};