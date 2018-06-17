#include "drawable.h"

Drawable::Drawable()
{
	setup();
}

Drawable::Drawable(GLchar* mmodel)
{
	objModel = Model(mmodel);
	size = objModel.get_size();
	setup();
}

Drawable::~Drawable()
{
}

void Drawable::dispose()
{
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
}

void Drawable::draw()
{
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	objModel.draw(shader, draw_type);
}

void Drawable::draw(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	objModel.draw(shader, draw_type);
}

void Drawable::draw_normals()
{
	glUniformMatrix4fv(glGetUniformLocation(shader_normals.get_id(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	objModel.draw_normals(shader_normals);
}

void Drawable::draw_normals(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	objModel.draw_normals(shader);
}

void Drawable::draw_box()
{
	shader_box.use();
	glUniform4f(glGetUniformLocation(shader_box.get_id(), "color"), 1.0f, 1.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader_box.get_id(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));

	glBindVertexArray(this->boxVAO);
	glDrawArrays(GL_LINES, 0, 36);
	glBindVertexArray(0);
}

void Drawable::draw_box(Shader shader)
{
	shader.use();
	glUniform4f(glGetUniformLocation(shader.get_id(), "color"), 1.0f, 1.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));

	glBindVertexArray(this->boxVAO);
	glDrawArrays(GL_LINES, 0, 30);
	glBindVertexArray(0);
}

void Drawable::setup()
{
	shader = Shaders[SHADER_BASIC];
	shader_normals = Shaders[SHADER_DEFFERED_NORMALS];
	shader_box = Shaders[SHADER_DEFFERED_GEOMETRY];

	glGenVertexArrays(1, &this->boxVAO);
	glGenBuffers(1, &this->boxVBO);

	update_model();
	load_box();
}

glm::mat4 Drawable::get_model_matrix()
{
	return mmodel;
}

void Drawable::rotate(GLfloat x, GLfloat y, GLfloat z)
{
	vrotation = glm::vec3(x, y, z);
	update_model();
}

void Drawable::rotate(glm::vec3 vrotation)
{
	this->vrotation = vrotation;
	update_model();
}

void Drawable::translate(GLfloat x, GLfloat y, GLfloat z)
{
	position += glm::vec3(x, y, z);
	update_model();
}

void Drawable::translate(glm::vec3 vtranslation)
{
	position += vtranslation;
	update_model();
}

void Drawable::scale(GLfloat x, GLfloat y, GLfloat z) {
	vscale = glm::vec3(x, y, z);
	update_model();

}

void Drawable::scale(glm::vec3 vscale)
{
	this->vscale = vscale;
	update_model();
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
	update_model();
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
	return position;
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

void Drawable::set_center(glm::vec3 center)
{
	this->vcenter = center;
	load_box();
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
	update_model();
	load_box();
}

void Drawable::set_shader(Shader shader)
{
	this->shader = shader;
}

unsigned int Drawable::get_dimension()
{
	return dimension;
}

void Drawable::update_model()
{
	mmodel = glm::mat4(1.0f);

	glm::vec3 center_translation = glm::vec3(
		vcenter.x * size.width * vscale.x, 
		vcenter.y * size.height * vscale.y,
		vcenter.z * size.depth * vscale.z);
	

	glm::quat rot_quat = glm::quat(glm::radians(vrotation));
	glm::mat4 rotation = glm::toMat4(rot_quat);

	rotation = glm::translate(glm::mat4(1.0f), center_translation) 
		* rotation 
		* glm::translate(glm::mat4(1.0f), -center_translation);


	mmodel = glm::translate(glm::mat4(1.0f), position) * rotation * glm::scale(glm::mat4(1.0f), vscale);

	dimension = 3;
	if (size.width * vscale.x == 0.0f) {
		dimension--;
	}
	if (size.height * vscale.y == 0.0f) {
		dimension--;
	}
	if (size.depth * vscale.z == 0.0f) {
		dimension--;
	}
}

void Drawable::load_box()
{
	GLfloat max_center_size = 0.0f;
	max_center_size = size.width;
	if (size.height > max_center_size) {
		max_center_size = size.height;
	}
	if (size.depth > max_center_size) {
		max_center_size = size.depth;
	}
	max_center_size *= 0.1f;
	GLfloat vertices_box[] = {
		size.x,				 size.y,			   size.z,
		size.x + size.width, size.y,			   size.z,
		size.x,				 size.y + size.height, size.z + size.depth,
		size.x + size.width, size.y + size.height, size.z + size.depth,
		size.x,				 size.y + size.height, size.z,
		size.x + size.width, size.y + size.height, size.z,
		size.x,				 size.y,			   size.z + size.depth,
		size.x + size.width, size.y,			   size.z + size.depth,

		size.x + size.width, size.y,			   size.z,
		size.x + size.width, size.y,			   size.z + size.depth,
		size.x + size.width, size.y + size.height, size.z,
		size.x + size.width, size.y + size.height, size.z + size.depth,
		size.x,				 size.y + size.height, size.z,
		size.x,				 size.y + size.height, size.z + size.depth,
		size.x,				 size.y,			   size.z,
		size.x,				 size.y,			   size.z + size.depth,

		size.x + size.width, size.y,			   size.z,
		size.x + size.width, size.y + size.height, size.z,
		size.x + size.width, size.y,			   size.z + size.depth,
		size.x + size.width, size.y + size.height, size.z + size.depth,
		size.x,				 size.y,			   size.z + size.depth,
		size.x,				 size.y + size.height, size.z + size.depth,
		size.x,				 size.y,			   size.z,
		size.x,				 size.y + size.height, size.z,
		
		//center axis
		vcenter.x * size.width - max_center_size, vcenter.y * size.height, vcenter.z * size.depth,
		vcenter.x * size.width + max_center_size, vcenter.y * size.height, vcenter.z * size.depth,

		vcenter.x * size.width, vcenter.y * size.height - max_center_size, vcenter.z * size.depth,
		vcenter.x * size.width, vcenter.y * size.height + max_center_size, vcenter.z * size.depth,

		vcenter.x * size.width, vcenter.y * size.height, vcenter.z * size.depth - max_center_size,
		vcenter.x * size.width, vcenter.y * size.height, vcenter.z * size.depth + max_center_size
	};

	glBindVertexArray(boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_box), &vertices_box, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}
