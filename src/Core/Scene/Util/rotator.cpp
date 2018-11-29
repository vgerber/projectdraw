#include "rotator.h"

Rotator::Rotator() {
	this->rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
}

Rotator::Rotator(glm::quat rotation) {
    this->rotation = rotation;
}

void Rotator::rotateEuler(float x, float y, float z) {
    rotateEuler(glm::vec3(x, y, z));
}

void Rotator::rotateEuler(glm::vec3 rotation) {
    this->rotation = glm::quat(rotation);
}

void Rotator::rotate(glm::quat rotation) {
    this->rotation = rotation;
}

void Rotator::rotateAxis(float radians, glm::vec3 rotationAxis) {
    rotation = glm::angleAxis(radians, rotationAxis);
}

void Rotator::applyRotation(glm::quat rotation) {
    this->rotation = rotation * this->rotation;
}

void Rotator::applyRotation(glm::vec3 euler) {
    applyRotation(glm::quat(euler));
}

void Rotator::applyRotation(Rotator rotator) {
    applyRotation(rotator.getRotation());
}

void Rotator::vectorRotation(glm::vec3 vecFrom, glm::vec3 vecTo) {
    glm::quat rotation;

    vecFrom = glm::normalize(vecFrom);
    vecTo = glm::normalize(vecTo);

    float cosTheta = glm::dot(vecFrom, vecTo);
    glm::vec3 rotationAxis;

    if(cosTheta < -1.0f + 0.001f) {
        //handle case when vectors ar opposite directions
        rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), vecFrom);
        //if vectors are aprallel
        if(glm::length2(rotationAxis) < 0.01) {
            rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), vecFrom);
        }

        rotationAxis = glm::normalize(rotationAxis);
        this->rotation =  glm::angleAxis(glm::radians(180.0f), rotationAxis);
        return;
    }

    rotationAxis = glm::cross(vecFrom, vecTo);

    float s = sqrt((1 + cosTheta) * 2);
    float invs = 1 / s;
    
    this->rotation =  glm::quat(
        s * 0.5f, 
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs);
}

void Rotator::interpolate(glm::quat rotation, float factor) {
    this->rotation = glm::mix(this->rotation, rotation, factor);
}

void Rotator::interpolate(Rotator rotator, float factor) {
    interpolate(rotator.getRotation(), factor);
}

glm::vec3 Rotator::getRotationAxis() {
    return glm::axis(rotation);
}

glm::vec3 Rotator::getRotationEuler() {
    return glm::eulerAngles(rotation);
}

glm::quat Rotator::getRotation() {
    return rotation;
}

glm::mat4 Rotator::getRotationMatrix() {
    return glm::toMat4(rotation);
}

bool Rotator::isEqual(Rotator rotator) {
    return isEqual(rotator.getRotation());
}

bool Rotator::isEqual(glm::quat rotation) {
    float matching = glm::dot(this->rotation, rotation);
    return (std::abs(matching - 1.0) < 0.001);
}

