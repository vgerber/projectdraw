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

	/*
	Stencil
	0: normal draw
	1: stencil optiona active draw
	2: xray draw
	3: outline draw
	*/
/*
	if (settings.outlineVisible || settings.xrayVisible) {
		glStencilFunc(GL_ALWAYS, 2, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
	}
	else {
		//only write on stencils less than 2
		
	}
*/
	if (settings.xrayVisible) {
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(cameraProj * cameraView * getModelMatrix()));
		glUniform1f(glGetUniformLocation(shader.getId(), "useLight"), settings.xrayUseLight);
		glUniform1i(glGetUniformLocation(shader.getId(), "enableCustomColor"), settings.xrayCustomColor);
		glm::vec4 color = settings.xrayColor;
		glUniform4f(glGetUniformLocation(shader.getId(), "customColor"), color.r, color.g, color.b, color.a);
		
		glEnable(GL_DEPTH_TEST);
		//set all occupied pixels to 1
		glStencilFunc(GL_GREATER, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_REPLACE, GL_KEEP);
		glStencilMask(0xFF);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		objModel.draw(shader, settings.xrayDrawType);

		//draw to all pixels with stencil equals 1 and reset it to zero
		glStencilFunc(GL_EQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilMask(0xFF);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glDisable(GL_DEPTH_TEST);		
		objModel.draw(shader, settings.xrayDrawType);		
		glEnable(GL_DEPTH_TEST);
	}
	

	if (settings.outlineVisible) {
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(cameraProj * cameraView * getModelMatrix()));
		glUniform1f(glGetUniformLocation(shader.getId(), "useLight"), 0.0f);
		glUniform1i(glGetUniformLocation(shader.getId(), "enableCustomColor"), 1);
		glm::vec4 color = settings.outlineColor;
		glUniform4f(glGetUniformLocation(shader.getId(), "customColor"), color.r, color.g, color.b, color.a);

		//set all occupied pixels to 2
		glStencilFunc(GL_GREATER, 2, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		objModel.draw(shader, DrawType::TRIANGLEG);



		Size outlineSize;
		Size size = getSize();	
		float thickness = settings.outlineThickness;
		outlineSize.width = (size.width + thickness) / size.width;
		outlineSize.height = (size.height + thickness) / size.height;
		outlineSize.depth = (size.depth + thickness) / size.depth;

		glm::vec3 oldScale = getScale();
		glm::vec3 oldPosition = getPosition();

		scale(outlineSize.width, outlineSize.height, outlineSize.depth);
		
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(cameraProj * cameraView * getModelMatrix()));
		//setPosition(getPosition() + glm::vec3(-0.5f * thickness));
		
		glStencilFunc(GL_EQUAL, 0, 0xFF);
		glStencilOp(GL_ZERO, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		
		objModel.draw(shader, DrawType::TRIANGLEG);		

		setPosition(oldPosition);
		scale(oldScale);
	}	

	glUniform1f(glGetUniformLocation(shader.getId(), "useLight"), 1.0f);
	glUniform1i(glGetUniformLocation(shader.getId(), "enableCustomColor"), 0);
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(cameraProj * cameraView * getModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
	
	glStencilFunc(GL_EQUAL, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	objModel.draw(shader, settings.drawType);	
}

void Drawable::drawRaw()
{
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(cameraProj * cameraView * getModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
	glUniform1f(glGetUniformLocation(shader.getId(), "useLight"), 0.0f);
	glUniform1i(glGetUniformLocation(shader.getId(), "enableCustomColor"), 0);
	objModel.draw(shader, settings.drawType);
}

void Drawable::drawNormals(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
	objModel.drawNormals(shader);
}

void Drawable::drawBox(Shader shader)
{
	shader.use();
	glUniform4f(glGetUniformLocation(shader.getId(), "color"), 0.0f, 1.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));

	glBindVertexArray(this->boxVAO);
	glDrawArrays(GL_LINES, 0, 30);
	glBindVertexArray(0);
}

void Drawable::setup()
{

	glGenVertexArrays(1, &this->boxVAO);
	glGenBuffers(1, &this->boxVBO);

	transform = Transform();
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
	objModel = model;
	size = objModel.getSize();
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
		/*vcenter.x * size.width - max_center_size, vcenter.y * size.height, vcenter.z * size.depth,
		vcenter.x * size.width + max_center_size, vcenter.y * size.height, vcenter.z * size.depth,

		vcenter.x * size.width, vcenter.y * size.height - max_center_size, vcenter.z * size.depth,
		vcenter.x * size.width, vcenter.y * size.height + max_center_size, vcenter.z * size.depth,

		vcenter.x * size.width, vcenter.y * size.height, vcenter.z * size.depth - max_center_size,
		vcenter.x * size.width, vcenter.y * size.height, vcenter.z * size.depth + max_center_size*/
	};

	glBindVertexArray(boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_box), &vertices_box, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}
