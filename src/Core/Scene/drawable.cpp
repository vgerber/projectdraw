#include "drawable.h"

Drawable::Drawable()
{
	setup();
}

Drawable::~Drawable()
{
}

void Drawable::dispose()
{
	objModel.dispose();
	glDeleteVertexArrays(1, &boxVAO);
	glDeleteBuffers(1, &boxVBO);
}

void Drawable::draw()
{
	shader.use();

	glUniform1f(glGetUniformLocation(shader.getId(), "useLight"), 1.0f);
	glUniform1i(glGetUniformLocation(shader.getId(), "enableCustomColor"), 0);
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(cameraProj * cameraView * mmodel));
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	
	/*
	Stencil
	0: normal draw
	1: stencil optiona active draw
	2: xray draw
	3: outline draw
	*/

	if (settings.outlineVisible || settings.xrayVisible) {
		glStencilFunc(GL_GREATER, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
	}
	else {
		//only write on stencils less than 2
		glStencilFunc(GL_GREATER, 2, 0xFF);
		glStencilMask(0x00);
	}
	
	objModel.draw(shader, settings.drawType);

	if (settings.xrayVisible) {
		glStencilFunc(GL_EQUAL, 0, 0xFF);
		glStencilOp(GL_KEEP, GL_REPLACE, GL_KEEP);
		glStencilMask(0xFF);

		glUniform1f(glGetUniformLocation(shader.getId(), "useLight"), settings.xrayUseLight);
		glUniform1i(glGetUniformLocation(shader.getId(), "enableCustomColor"), settings.xrayCustomColor);
		glm::vec4 color = settings.xrayColor;
		glUniform4f(glGetUniformLocation(shader.getId(), "customColor"), color.r, color.g, color.b, color.a);
		
		glDisable(GL_DEPTH_TEST);
		objModel.draw(shader, settings.xrayDrawType);
		glEnable(GL_DEPTH_TEST);
	}
	

	if (settings.outlineVisible) {
		Size outlineSize;
		Size size = getSize();

		float thickness = settings.outlineThickness;
		outlineSize.width = (size.width + thickness) / size.width;
		outlineSize.height = (size.height + thickness) / size.height;
		outlineSize.depth = (size.depth + thickness) / size.depth;

		glm::vec3 oldScale = getScale();
		glm::vec3 oldPosition = getPosition();

		scale(outlineSize.width, outlineSize.height, outlineSize.depth);
		
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
		glStencilFunc(GL_EQUAL, 0, 0xFF);
		glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
		glStencilMask(0xFF);
		
		//setPosition(getPosition() + glm::vec3(-0.5f * thickness));
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(cameraProj * cameraView * mmodel));
		glUniform1f(glGetUniformLocation(shader.getId(), "useLight"), 0.0f);
		glUniform1i(glGetUniformLocation(shader.getId(), "enableCustomColor"), 1);
		glm::vec4 color = settings.outlineColor;
		glUniform4f(glGetUniformLocation(shader.getId(), "customColor"), color.r, color.g, color.b, color.a);

		objModel.draw(shader, settings.drawType);

		setPosition(oldPosition);
		scale(oldScale);
	}
	
	glStencilMask(0xFF);
	
}

void Drawable::drawRaw()
{
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(cameraProj * cameraView * mmodel));
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	glUniform1f(glGetUniformLocation(shader.getId(), "useLight"), 0.0f);
	glUniform1i(glGetUniformLocation(shader.getId(), "enableCustomColor"), 0);
	objModel.draw(shader, settings.drawType);
}

void Drawable::drawNormals(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mmodel));
	objModel.drawNormals(shader);
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

	glGenVertexArrays(1, &this->boxVAO);
	glGenBuffers(1, &this->boxVBO);

	updateModel();
	loadBox();
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

void Drawable::setCenter(glm::vec3 center)
{
	Moveable::setCenter(center);
	loadBox();
}

void Drawable::setCenterInWorld(glm::vec3 point)
{
	Moveable::setCenterInWorld(point);
	loadBox();
}

void Drawable::setCameraMatrices(glm::mat4 cView, glm::mat4 cProj)
{
	cameraView = cView;
	cameraProj = cProj;
}

Model Drawable::getModel()
{
	return objModel;
}

Model * Drawable::getModelPtr()
{
	return &objModel;
}

void Drawable::setModel(Model model)
{
	vscale = glm::vec3(1.0f);
	objModel = model;
	size = objModel.getSize();
	updateModel();
	loadBox();
}

void Drawable::setShader(Shader shader, AbstractRenderer &renderer)
{
	
	this->currentRenderer = renderer.getRendererType();
	this->shader = shader;
	if (renderer.getRendererType() != currentRenderer) {
		renderer.invalidateShader();
	}
}

std::pair<Shader, int> Drawable::getShader()
{
	return std::pair<Shader, int>(shader, currentRenderer);
}

unsigned int Drawable::getDimension()
{
	return dimension;
}



void Drawable::loadBox()
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
