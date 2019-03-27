#include "freecamera.h"


void FreeCamera::moveUp(float value) {
    translate(getTranslation() + getUp() * value * moveSpeed);
}

void FreeCamera::moveRight(float value) {
    translate(getTranslation() + getRight() * value * moveSpeed);
}

void FreeCamera::moveForward(float value) {
    translate(getTranslation() + getForward() * value * moveSpeed);
}

void FreeCamera::tiltUp(float value) {
    if(abs(pitch + value * tiltSpeed) < maxUpAngle) {
        pitch += value * tiltSpeed;
        Rotator rotator = Rotator(pitch, getBaseRight());
        rotator.applyRotation(Rotator(yaw, getBaseUp()));
        rotate(rotator);
    }
}

void FreeCamera::tiltRight(float value) {
    yaw += value * tiltSpeed;
    Rotator rotator = Rotator(pitch, getBaseRight());
    rotator.applyRotation(Rotator(yaw, getBaseUp()));
    rotate(rotator);
}

void FreeCamera::setMoveSpeed(float speed) {
    this->moveSpeed = speed;
}

void FreeCamera::setTiltSpeed(float speed) {
    this->tiltSpeed = speed;
}

void FreeCamera::setMaxPitch(float radians) {
    maxUpAngle = radians;
    if(maxUpAngle > abs(pitch)) {
        pitch = maxUpAngle;
        Rotator rotator = Rotator(pitch, getBaseRight());
        rotator.applyRotation(Rotator(yaw, getBaseUp()));
        rotate(rotator);
    }
}