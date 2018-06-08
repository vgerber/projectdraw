#include "drawable.h"

/*
Drawable::Drawable()
{
	
	//Linux
	shader.Load("/home/vincent/Development/Cpp/opengl/ProjectDraw/src/Shaders/basic.vertex", 
                "/home/vincent/Development/Cpp/opengl/ProjectDraw/src/Shaders/basic.fragment");
	objModel = Model("/home/vincent/Development/Cpp/opengl/basic_mountain.obj");
	

	//Windows
	shader.Load("C:/Users/Vincent/Documents/Projects/C++/ProjectDraw/src/Shaders/basic.vertex",
		"C:/Users/Vincent/Documents/Projects/C++/ProjectDraw/src/Shaders/basic.fragment");
	objModel = Model("C:/Users/Vincent/Documents/Projects/Blender/TriFace/basic_mountain.obj");
}
*/

Drawable::Drawable()
{
}

Drawable::Drawable(GLchar* model)
{
	objModel = Model(model);
}

Drawable::~Drawable()
{
}

void Drawable::Dispose()
{
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
}

void Drawable::Draw(Shader shader)
{
	objModel.Draw(shader);
}

glm::mat4 Drawable::GetModelMatrix()
{
	return model;
}

void Drawable::Rotate(GLfloat x, GLfloat y, GLfloat z)
{
	rotation = glm::vec3(x, y, z);
	updateModel();
}

void Drawable::Rotate(glm::vec3 rotation)
{
	this->rotation = rotation;
	updateModel();
}

void Drawable::Translate(GLfloat x, GLfloat y, GLfloat z)
{
	translation = glm::vec3(x, y, z);
	updateModel();
}

void Drawable::Translate(glm::vec3 translation)
{
	this->translation = translation;
	updateModel();
}

void Drawable::Scale(GLfloat x, GLfloat y, GLfloat z) {
	scale = glm::vec3(x, y, z);
	updateModel();

}

void Drawable::Scale(glm::vec3 scale)
{
	this->scale = scale;
	updateModel();
}

glm::vec3 Drawable::GetPosition()
{
	auto pos = model[3];
	return glm::vec3(pos.x, pos.y, pos.z);
}

glm::vec3 Drawable::GetScale()
{
	return scale;
}

glm::vec3 Drawable::GetRotion()
{
	return rotation;
}

void Drawable::SetPosition(glm::vec3 position)
{
	translation = translation - position;
	updateModel();
}

void Drawable::updateModel()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, translation);
	model = glm::scale(model, scale);
	if (rotation.x != 0.0f) {
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (rotation.y != 0.0f) {
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (rotation.z != 0.0f) {
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	}
}
