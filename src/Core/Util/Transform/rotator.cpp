#include "rotator.h"

Rotator::Rotator() {
	this->rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	this->origin = glm::vec3(0.0f, 0.0f, 0.0f);
}

Rotator::Rotator(glm::quat rotation, glm::vec3 origin)
{
	this->rotation = rotation;
	this->origin = origin;
}

Rotator::Rotator(float radians, glm::vec3 rotationAxis) {
    rotateAxis(radians, rotationAxis);
    this->origin = glm::vec3(0.0f, 0.0f, 0.0f);
}

Rotator::Rotator(float x, float y, float z) {
    rotateEuler(x, y, z);
    this->origin = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Rotator::setOrigin(float x, float y, float z)
{
	setOrigin(glm::vec3(x, y, z));
}

void Rotator::setOrigin(glm::vec3 origin)
{
	this->origin = origin;
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

glm::vec3 Rotator::rotatePoint(glm::vec3 point)
{	
	return getRotationMatrix() * glm::vec4(point, 0.0f);
}

void Rotator::applyRotation(glm::quat rotation) {
    this->rotation = rotation * this->rotation;
}

void Rotator::applyRotation(glm::vec3 euler) {
    applyRotation(glm::quat(euler));
}

void Rotator::applyRotation(Rotator rotator) {
	appliedMatrices.push_back(rotator.getRotationMatrix());
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
    //this->rotation = glm::rotation(vecFrom, vecTo);
    //rotateEuler(getRotationEuler() * glm::vec3(0.0f, 1.0f, 1.0f));
}

void Rotator::interpolate(glm::quat rotation, float factor) {
    this->rotation = glm::mix(this->rotation, rotation, factor);
}

void Rotator::interpolate(Rotator rotator, float factor) {
    interpolate(rotator.getRotation(), factor);
}

glm::vec3 Rotator::getOrigin() const {
	return origin;
}

glm::vec3 Rotator::getRotationAxis() const {
    return glm::axis(rotation);
}

glm::vec3 Rotator::getRotationEuler() const {
    return glm::eulerAngles(rotation);
}

glm::quat Rotator::getRotation() const {
    return rotation;
}

glm::mat4 Rotator::getRotationMatrix() {
	updateMatrix();
	glm::mat4 finalRotationMatrix = glm::mat4(1.0);
	
	for (int i = 0; i < appliedMatrices.size(); i++) {
		finalRotationMatrix = appliedMatrices[i] * finalRotationMatrix;
	}
	finalRotationMatrix *= rotationMatrix;
    return finalRotationMatrix;
}

bool Rotator::isEqual(Rotator rotator) {
    return isEqual(rotator.getRotation());
}

bool Rotator::isEqual(glm::quat rotation) {
    float matching = glm::dot(this->rotation, rotation);
    return (std::abs(matching - 1.0) < 0.001);
}

void Rotator::clearAppliedRotators()
{
	appliedMatrices.clear();
}

void Rotator::updateMatrix()
{
	rotationMatrix = glm::translate(glm::mat4(1.0), origin) * glm::toMat4(rotation) * glm::translate(glm::mat4(1.0), -origin);
}

