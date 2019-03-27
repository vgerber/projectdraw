#include "moveable.h"

Transform::Transform() {
	
}

Transform::Transform(glm::vec3 translation, Rotator rotator, glm::vec3 scale) {
	this->translation = translation;
	this->rotator = rotator;
	this->scaling = scale;
	updateMatrix();
}

Transform::Transform(glm::mat4 transform) {
	glm::quat rotation;
	glm::vec3 skew;
	glm::vec4 persp;
	glm::decompose(transform, this->scaling, rotation, this->translation, skew, persp);
	this->rotator = Rotator(rotation, glm::vec3(0.0));
	updateMatrix();
}

void Transform::translate(glm::vec3 translation) {
	this->translation = translation;
	updateMatrix();
}

void Transform::scale(glm::vec3 scaling) {
	this->scaling = scaling;
	updateMatrix();
}

void Transform::rotate(Rotator rotator) {
	this->rotator = rotator;
	updateMatrix();
}

glm::vec3 Transform::getPosition() {
	return position;
}

glm::vec3 Transform::getTranslation() {
	return translation;
}

glm::vec3 Transform::getScale() {
	return scaling;
}

Rotator Transform::getRotation() {
	return rotator;
}

glm::mat4 Transform::getInverse() {
	return glm::inverse(transformMatrix);
}

glm::mat4 Transform::getMatrix() {
	return transformMatrix;
}

Transform Transform::operator*(const Transform & transform)
{
	glm::vec3 translation;
	glm::vec3 scaling;
	glm::quat rotation;
	glm::vec3 skew;
	glm::vec4 persp;
	glm::mat4 combinedMatrix = transform.transformMatrix * this->transformMatrix;
	glm::decompose(combinedMatrix, scaling, rotation, translation, skew, persp);
	return Transform(translation, Rotator(rotation, glm::vec3(0.0f)), scaling);
}

void Transform::print() {
	printf("Transfrom\n");
	printf("Translation (%f, %f, %f)\n", translation.x, translation.y, translation.z);
	glm::vec3 rotation = rotator.getRotationEuler();
	printf("Rotation RAD(%f, %f, %f) DEG(%f, %f, %f)\n", rotation.x, rotation.y, rotation.z, glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z));
	printf("Scale (%f, %f, %f)\n", scaling.x, scaling.y, scaling.z);
}

void Transform::updateMatrix() {
	transformMatrix =
		glm::translate(glm::mat4(1.0), translation) *
		rotator.getRotationMatrix() *
		glm::scale(glm::mat4(1.0), scaling);
	position = transformMatrix * glm::vec4(0, 0, 0, 1);
}

Transform Moveable::getTransform() {
	return transform;
}

void Moveable::translate(float x, float y, float z)
{
	translate(glm::vec3(x, y, z));
}

void Moveable::translate(glm::vec3 translation)
{
	transform.translate(translation);
	transformChanged();
}

void Moveable::rotate(Rotator rotator)
{
	transform.rotate(rotator);
	transformChanged();
}

void Moveable::scale(float x, float y, float z) {
	scale(glm::vec3(x, y, z));
}

void Moveable::scale(glm::vec3 scaling)
{
	transform.scale(scaling);
	transformChanged();
}

void Moveable::setTransform(Transform transform) {
	this->transform = transform;
	transformChanged();
}

void Moveable::setForward(glm::vec3 forwardDirection) {
	this->forward = glm::normalize(forwardDirection);
	this->right = glm::cross(this->forward, this->up);
	if(isRightFlipped())
		this->right = -this->right;
	updateDirection(transform);
}

void Moveable::setUp(glm::vec3 upDirection) {
	this->up = glm::normalize(upDirection);
	this->right = glm::cross(this->forward, this->up);
	if(isRightFlipped())
		this->right = -this->right;
	updateDirection(transform);
}

void Moveable::setForwardUp(glm::vec3 forwardDircetion, glm::vec3 upDirection)
{
	this->forward = glm::normalize(forwardDircetion);
	this->up = glm::normalize(upDirection);
	this->right = glm::cross(this->forward, this->up);
	if(isRightFlipped())
		this->right = -this->right;
	updateDirection(transform);
}

void Moveable::flipRight()
{
	this->right = -this->right;
	rightFlipped = !rightFlipped;
	updateDirection(transform);
}

glm::vec3 Moveable::getForward() {
	return transForward;
}

glm::vec3 Moveable::getUp() {
	return transUp;
}

glm::vec3 Moveable::getRight() {
	return transRight;
}

glm::vec3 Moveable::getBaseForward() {
	return forward;
}

glm::vec3 Moveable::getBaseUp() {
	return up;
}

glm::vec3 Moveable::getBaseRight() {
	return right;
}

Size Moveable::getSize()
{
	glm::vec3 scaling = transform.getScale();
	Size scaled_size;
	scaled_size = size;
	scaled_size.width *= scaling.x;
	scaled_size.height *= scaling.y;
	scaled_size.depth *= scaling.z;
	return scaled_size;
}

void Moveable::scaleToSize(Size size)
{
	if (std::isnan(size.width)) {
		size.width = 1.0f;
	}
	if (std::isnan(size.height)) {
		size.height = 1.0f;
	}
	if (std::isnan(size.depth)) {
		size.depth = 1.0f;
	}
	glm::vec3 scaling = transform.getScale();
	if (this->size.width > 0.0f)
		scaling.x = (size.width / this->size.width);
	if (this->size.height > 0.0f)
		scaling.y = (size.height / this->size.height);
	if (this->size.depth > 0.0f)
		scaling.z = (size.depth / this->size.depth);
	transform.scale(scaling);
	transformChanged();
}

void Moveable::scaleToWidth(float width)
{
	float scale_size = width / size.width;
	Size new_size;
	new_size.width = size.width * scale_size;
	new_size.height = size.height * scale_size;
	new_size.depth = size.depth * scale_size;
	scaleToSize(new_size);
}

void Moveable::scaleToHeight(float height)
{
	float scale_size = height / size.height;
	Size new_size;
	new_size.width = size.width * scale_size;
	new_size.height = size.height * scale_size;
	new_size.depth = size.depth * scale_size;
	scaleToSize(new_size);
}

void Moveable::scaleToDepth(float depth)
{
	float scale_size = depth / size.depth;
	Size new_size;
	new_size.width = size.width * scale_size;
	new_size.height = size.height * scale_size;
	new_size.depth = size.depth * scale_size;
	scaleToSize(new_size);
}

glm::vec3 Moveable::getPosition() {
	return transform.getPosition();
}

glm::vec3 Moveable::getTranslation()
{
	return transform.getTranslation();
}

Rotator Moveable::getRotator()
{
	return transform.getRotation();
}

glm::vec3 Moveable::getScale()
{
	return transform.getScale();
}

void Moveable::transformChanged() {
	updateDirection(transform);
}

void Moveable::updateDirection(Transform transform) {
	assert(glm::length2(forward) != 0.0f);
	assert(glm::length2(up) != 0.0f);
	assert(glm::length2(right) != 0.0f);
	if(glm::length2(forward) == 0.0f) {
		printf("[Warning] Forward is zero\n");
	}
	if(glm::length2(up) == 0.0f) {
		printf("[Warning] Up is zero\n");
	}
	if(glm::length2(right) == 0.0f) {
		printf("[Warning] Right is zero\n");
	}

	glm::mat4 rotationMatrix = transform.getRotation().getRotationMatrix();	
	transForward = rotationMatrix * glm::vec4(forward, 0.0);
	transUp      = rotationMatrix * glm::vec4(up, 0.0);
	transRight   = rotationMatrix * glm::vec4(right, 0.0);	
}

bool Moveable::isRightFlipped() {
	return rightFlipped;
}