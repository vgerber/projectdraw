#include "drawable.h"

Drawable::Drawable()
{
	setup();
}

Drawable::Drawable(GLchar* mmodel)
{
	objModel = Model(mmodel);
	size = objModel.getSize();
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
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	objModel.draw(shader, drawType);
}

void Drawable::draw(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	objModel.draw(shader, drawType);
}

void Drawable::drawNormals()
{
	glUniformMatrix4fv(glGetUniformLocation(shader_normals.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	objModel.drawNormals(shader_normals);
}

void Drawable::drawNormals(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	objModel.drawNormals(shader);
}

void Drawable::drawBox()
{
	drawBox(shader_box);
}

void Drawable::drawBox(Shader shader)
{
	shader.use();
	glUniform4f(glGetUniformLocation(shader.getId(), "color"), 0.0f, 1.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));

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

glm::mat4 Drawable::getModelMatrix()
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

Size Drawable::getSize()
{
	Size scaled_size;
	scaled_size = size;
	scaled_size.width *= vscale.x;
	scaled_size.height *= vscale.y;
	scaled_size.depth *= vscale.z;
	return scaled_size;
}

Size Drawable::getAABBBox()
{
	glm::vec4 box[8] = {
		mmodel * glm::vec4(size.x,				size.y,				  size.z,			   1.0f),
		mmodel * glm::vec4(size.x + size.width, size.y,				  size.z,			   1.0f),
		mmodel * glm::vec4(size.x,				size.y + size.height, size.z,			   1.0f),
		mmodel * glm::vec4(size.x + size.width, size.y + size.height, size.z,			   1.0f),
		mmodel * glm::vec4(size.x,				size.y,				  size.z + size.depth, 1.0f),
		mmodel * glm::vec4(size.x + size.width, size.y,				  size.z + size.depth, 1.0f),
		mmodel * glm::vec4(size.x,				size.y + size.height, size.z + size.depth, 1.0f),
		mmodel * glm::vec4(size.x + size.width,	size.y + size.height, size.z + size.depth, 1.0f)
	};
	Size box_size;
	bool is_first = true;
	for (auto v : box) {
		//v *= glm::vec4(vscale, 1.0f);
		if (is_first) {
			box_size.x = v.x;
			box_size.y = v.y;
			box_size.z = v.z;
			box_size.width = v.x;
			box_size.height = v.y;
			box_size.depth = v.z;			
			is_first = false;
			continue;
		}
		if (v.x < box_size.x) {
			box_size.x = v.x;
		}
		//max width
		if (v.x > box_size.width) {
			box_size.width = v.x;
		}
		//min y
		if (v.y < box_size.y) {
			box_size.y = v.y;
		}
		//max height
		if (v.y > box_size.height) {
			box_size.height = v.y;
		}
		//min z
		if (v.z < box_size.z) {
			box_size.z = v.z;
		}
		//max length
		if (v.z > box_size.depth) {
			box_size.depth = v.z;
		}
	}
	box_size.width -= box_size.x;
	box_size.height -= box_size.y;
	box_size.depth -= box_size.z;
	return box_size;
}

void Drawable::scaleToSize(Size size)
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
	if(this->size.width > 0.0f)
		vscale.x = (size.width / this->size.width);
	if(this->size.height > 0.0f)
		vscale.y = (size.height / this->size.height);
	if(this->size.depth > 0.0f)
		vscale.z = (size.depth / this->size.depth);
	update_model();
}

void Drawable::scaleToWidth(GLfloat width)
{
	GLfloat scale_size = width / size.width;
	Size new_size;
	new_size.width = size.width * scale_size;
	new_size.height = size.height * scale_size;
	new_size.depth = size.depth * scale_size;
	scaleToSize(new_size);
}

void Drawable::scaleToHeight(GLfloat height)
{
	GLfloat scale_size = height / size.height;
	Size new_size;
	new_size.width = size.width * scale_size;
	new_size.height = size.height * scale_size;
	new_size.depth = size.depth * scale_size;
	scaleToSize(new_size);
}

void Drawable::scaleToLength(GLfloat depth)
{
	GLfloat scale_size = depth / size.depth;
	Size new_size;
	new_size.width = size.width * scale_size;
	new_size.height = size.height * scale_size;
	new_size.depth = size.depth * scale_size;
	scaleToSize(new_size);
}

glm::vec3 Drawable::getPosition()
{
	return position;
}

glm::vec3 Drawable::getPositionCenter() {
	return glm::vec3(position.x + size.width * .5f, position.y + size.height * .5f, position.z + size.depth * .5f);
}

glm::vec3 Drawable::getScale()
{
	return vscale;
}

glm::vec3 Drawable::getRotation()
{
	return vrotation;
}

void Drawable::setPosition(glm::vec3 position)
{
	translate(position - this->position);
}

void Drawable::setPositionCenter(glm::vec3 position)
{
	setPosition(glm::vec3(position.x - size.width * .5f, position.y - size.height * .5f, position.z - size.depth * .5f));
}

void Drawable::setCenter(glm::vec3 center)
{
	this->vcenter = center;
	load_box();
}

void Drawable::transform(btTransform transform)
{
	btScalar rot_x, rot_y, rot_z;
	transform.getRotation().getEulerZYX(rot_z, rot_y, rot_x);
	rotate(rot_x, rot_y, rot_z);
	btVector3 pos = transform.getOrigin();
	setPositionCenter(glm::vec3(pos.getX(), pos.getY(), pos.getZ()));
}

Model Drawable::get_model()
{
	return objModel;
}

void Drawable::set_model(Model model)
{
	vscale = glm::vec3(1.0f);
	objModel = model;
	size = objModel.getSize();
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
	
	
	glm::quat rot_quat = glm::quat(vrotation);
	glm::mat4 rotation = glm::toMat4(rot_quat);

	rotation = glm::translate(glm::mat4(1.0f), center_translation) 
		* rotation 
		* glm::translate(glm::mat4(1.0f), -center_translation);

	
/*
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation = glm::rotate(rotation, vrotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, vrotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(rotation, vrotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
*/
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
