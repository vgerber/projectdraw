#include "moveable.h"

Transform::Transform() {
	
}

Transform::Transform(glm::vec3 translation, Rotator rotator, glm::vec3 scale) {
	this->translation = translation;
	this->rotator = rotator;
	this->scaling = scale;
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
	return glm::inverse(transfromMatrix);
}

glm::mat4 Transform::getMatrix() {
	return transfromMatrix;
}

Transform Transform::operator*(const Transform & transform)
{
	glm::vec3 translation;
	glm::vec3 scaling;
	glm::quat rotation;
	glm::decompose(this->transfromMatrix * transform.transfromMatrix, scaling, rotation, translation, glm::vec3(), glm::vec4());
	return Transform(translation, Rotator(rotation, glm::vec3(0.0f)), scaling);
}

void Transform::print() {
	printf("Transfrom\n");
	printf("Translation (%f, %f, %f)\n", translation.x, translation.y, translation.z);
	glm::vec3 rotation = rotator.getRotationEuler();
	printf("Rotation (%f, %f, %f) / (%f, %f, %f)\n", rotation.x, rotation.y, rotation.z, glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z));
	printf("Scale (%f, %f, %f)\n", scaling.x, scaling.y, scaling.z);
}

void Transform::updateMatrix() {
	transfromMatrix =
		glm::translate(glm::mat4(1.0), translation) *
		rotator.getRotationMatrix() *
		glm::scale(glm::mat4(1.0), scaling);
}


glm::mat4 Moveable::getModelMatrix()
{
	return transform.getMatrix();
}

void Moveable::setPosition(float x, float y, float z)
{
	transform.translate(glm::vec3(x, y, z));
}

void Moveable::setPosition(glm::vec3 translation)
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
	transform.scale(glm::vec3(x, y, z));
}

void Moveable::scale(glm::vec3 scaling)
{
	transform.scale(scaling);
	transformChanged();
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

void Moveable::scaleToLength(float depth)
{
	float scale_size = depth / size.depth;
	Size new_size;
	new_size.width = size.width * scale_size;
	new_size.height = size.height * scale_size;
	new_size.depth = size.depth * scale_size;
	scaleToSize(new_size);
}

glm::vec3 Moveable::getPosition()
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
	
}