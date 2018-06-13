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

Drawable::Drawable(GLchar* mmodel)
{
	objModel = Model(mmodel);
	size = objModel.get_size();
}

Drawable::~Drawable()
{
}

void Drawable::dispose()
{
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
}

void Drawable::draw(Shader shader)
{
	objModel.draw(shader);
}

void Drawable::draw_normals(Shader shader)
{
	objModel.draw_normals(shader);
}

glm::mat4 Drawable::get_model_matrix()
{
	return mmodel;
}

void Drawable::rotate(GLfloat x, GLfloat y, GLfloat z)
{
	vrotation = glm::vec3(x, y, z);
	updateModel();
}

void Drawable::rotate(glm::vec3 vrotation)
{
	this->vrotation = vrotation;
	updateModel();
}

void Drawable::translate(GLfloat x, GLfloat y, GLfloat z)
{
	vtranslation = glm::vec3(x, y, z);
	updateModel();
}

void Drawable::translate(glm::vec3 vtranslation)
{
	this->vtranslation = vtranslation;
	updateModel();
}

void Drawable::scale(GLfloat x, GLfloat y, GLfloat z) {
	vscale = glm::vec3(x, y, z);
	updateModel();

}

void Drawable::scale(glm::vec3 vscale)
{
	this->vscale = vscale;
	updateModel();
}

Size Drawable::get_size()
{
	return size;
}

void Drawable::scale_to_size(Size size)
{
	vscale.x = (size.width / this->size.width);
	vscale.y = (size.height / this->size.height);
	vscale.z = (size.depth / this->size.depth);
	this->size = size;
	updateModel();
}

void Drawable::scale_to_width(GLfloat width)
{
	GLfloat scale_size = width / size.width;
	Size new_size;
	new_size.width = size.width * scale_size;
	new_size.height = size.height * scale_size;
	new_size.depth = size.depth * scale_size;
	scale_to_size(new_size);
}

void Drawable::scale_to_height(GLfloat height)
{
	GLfloat scale_size = height / size.height;
	Size new_size;
	new_size.width = size.width * scale_size;
	new_size.height = size.height * scale_size;
	new_size.depth = size.depth * scale_size;
	scale_to_size(new_size);
}

void Drawable::scale_to_length(GLfloat depth)
{
	GLfloat scale_size = depth / size.depth;
	Size new_size;
	new_size.width = size.width * scale_size;
	new_size.height = size.height * scale_size;
	new_size.depth = size.depth * scale_size;
	scale_to_size(new_size);
}

glm::vec3 Drawable::get_position()
{
	auto pos = mmodel[3];
	return glm::vec3(pos.x, pos.y, pos.z);
}

glm::vec3 Drawable::get_scale()
{
	return vscale;
}

glm::vec3 Drawable::get_rotation()
{
	return vrotation;
}

void Drawable::set_position(glm::vec3 position)
{
	translate(position - this->position);
}

Model Drawable::get_model()
{
	return objModel;
}

void Drawable::set_model(Model model)
{
	vscale = glm::vec3(1.0f);
	objModel = model;
	size = objModel.get_size();
}

void Drawable::updateModel()
{
	mmodel = glm::mat4(1.0f);
	mmodel = glm::translate(mmodel, vtranslation);
	mmodel = glm::scale(mmodel, vscale);
	if (vrotation.x != 0.0f) {
		mmodel = glm::rotate(mmodel, vrotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (vrotation.y != 0.0f) {
		mmodel = glm::rotate(mmodel, vrotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (vrotation.z != 0.0f) {
		mmodel = glm::rotate(mmodel, vrotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	position = get_position();
}
