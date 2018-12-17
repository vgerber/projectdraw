#include "moveable.h"

glm::mat4 Moveable::getModelMatrix()
{
	return mmodel;
}


void Moveable::setCenter(glm::vec3 center)
{
	this->vcenter = center;
}

void Moveable::setPosition(glm::vec3 position)
{
	translate(position - this->position);
}

void Moveable::rotate(Rotator rotator)
{
	this->rotator = rotator;
	updateModel();
}

void Moveable::translate(float x, float y, float z)
{
	position += glm::vec3(x, y, z);
	updateModel();
}

void Moveable::translate(glm::vec3 vtranslation)
{
	position += vtranslation;
	updateModel();
}

void Moveable::scale(float x, float y, float z) {
	vscale = glm::vec3(x, y, z);
	updateModel();

}

void Moveable::scale(glm::vec3 vscale)
{
	this->vscale = vscale;
	updateModel();
}

void Moveable::setCenterInWorld(glm::vec3 point)
{
	glm::vec3 dPoint = point - position;
	vcenter.x = dPoint.x / (size.width != 0.0f ? size.width : 1.0f);
	vcenter.y = dPoint.y / (size.height != 0.0f ? size.height : 1.0f);
	vcenter.z = dPoint.z / (size.depth != 0.0f ? size.depth : 1.0f);
}

Size Moveable::getSize()
{
	Size scaled_size;
	scaled_size = size;
	scaled_size.width *= vscale.x;
	scaled_size.height *= vscale.y;
	scaled_size.depth *= vscale.z;
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
	if (this->size.width > 0.0f)
		vscale.x = (size.width / this->size.width);
	if (this->size.height > 0.0f)
		vscale.y = (size.height / this->size.height);
	if (this->size.depth > 0.0f)
		vscale.z = (size.depth / this->size.depth);
	updateModel();
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


glm::vec3 Moveable::getCenter()
{
	return vcenter;
}

glm::vec3 Moveable::getCenterPoint()
{
	return position + glm::vec3(size.width, size.height, size.depth) * vcenter;
}

glm::vec3 Moveable::getPosition()
{
	return position;
}

glm::vec3 Moveable::getPositionCenter() {
	return position + glm::vec3(size.width, size.height, size.depth) * glm::vec3(0.5f, 0.5f, 0.5f) * vscale;
}

Rotator Moveable::getRotator()
{
	return rotator;
}

void Moveable::print()
{
	printf("Transfrom\n");
	printf("Translation (%f, %f, %f)\n", position.x, position.y, position.z);
	glm::vec3 rotation = rotator.getRotationEuler();
	printf("Rotation (%f, %f, %f) / (%f, %f, %f)\n", rotation.x, rotation.y, rotation.z, glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z));
	printf("Scale (%f, %f, %f)\n", vscale.x, vscale.y, vscale.z);
}

glm::vec3 Moveable::getScale()
{
	return vscale;
}


void Moveable::updateModel()
{
	mmodel = glm::mat4(1.0f);
	/*
	glm::vec3 center_translation = glm::vec3(
		vcenter.x * size.width * vscale.x,
		vcenter.y * size.height * vscale.y,
		vcenter.z * size.depth * vscale.z);
	*/
	glm::vec3 center_translation = glm::vec3(
		size.x,
		size.y,
		size.z
	);
	glm::mat4 rotation = rotator.getRotationMatrix();
	/*
		glm::translate(glm::mat4(1.0f), rotator.getOrigin()) *
		rotator.getRotationMatrix() * 
		glm::translate(glm::mat4(1.0f), -rotator.getOrigin());
	*/

	/*rotation = glm::translate(glm::mat4(1.0f), center_translation)
		* rotation
		* glm::translate(glm::mat4(1.0f), -center_translation);
	*/

	/*
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation = glm::rotate(rotation, vrotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, vrotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(rotation, vrotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	*/
	mmodel = glm::translate(glm::mat4(1.0f), position) * rotation * glm::scale(glm::mat4(1.0f), vscale);
}
