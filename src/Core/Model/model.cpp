#include "model.h"



Transform Model::getModelBaseTransform()
{
	return baseTransform;
}

Transform Model::getTreeBaseTransfrom()
{
	if (this->parent) {
		return baseTransform * parent->getTreeBaseTransfrom();
	}
	return baseTransform;
}

void Model::applyBaseTransform(Transform transform)
{
	baseTransform = baseTransform * transform;
}

void Model::draw()
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

		drawTree(shader, settings.xrayDrawType);

		//draw to all pixels with stencil equals 1 and reset it to zero
		glStencilFunc(GL_EQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilMask(0xFF);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glDisable(GL_DEPTH_TEST);
		drawTree(shader, settings.xrayDrawType);
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

		drawTree(shader, DrawType::TRIANGLEG);



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

		drawTree(shader, DrawType::TRIANGLEG);

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
	drawTree(shader, settings.drawType);
}

void Model::drawRaw()
{
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(cameraProj * cameraView * getModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
	glUniform1f(glGetUniformLocation(shader.getId(), "useLight"), 0.0f);
	glUniform1i(glGetUniformLocation(shader.getId(), "enableCustomColor"), 0);
	drawTree(shader, settings.drawType);
}

void Model::drawInstancing(Shader shader, DrawType drawType, int amount)
{
}

void Model::drawNormals(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
	drawTreeNormals(shader);
}

void Model::drawBox(Shader shader)
{
	/*
	shader.use();
	glUniform4f(glGetUniformLocation(shader.getId(), "color"), 0.0f, 1.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));

	glBindVertexArray(this->boxVAO);
	glDrawArrays(GL_LINES, 0, 30);
	glBindVertexArray(0);
	*/
}

void Model::setCameraMatrices(glm::mat4 cView, glm::mat4 cProj)
{
	this->cameraProj = cProj;
	this->cameraView = cView;
	for (auto model : children) {
		model->setCameraMatrices(cView, cProj);
	}
}

void Model::dispose() {
	for (Model * model : children) {
		model->dispose();	
		delete model;
	}
}

GLint TextureFromFile(const char* path, std::string directory)
{
	/*
	//Generate texture ID and load texture data 
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
	*/
	return -1;
}

void Model::setParent(Model * parent) {
	this->parent = parent;
}

void Model::drawTree(Shader shader, DrawType type)
{
	drawModel(shader, type);
	for (Model * model : children) {
		model->drawTree(shader, type);
	}
}

void Model::drawTreeInstancing(Shader shader, DrawType type, int amount)
{
	drawModelInstancing(shader, type, amount);
	for (Model * model : children) {
		model->drawTreeInstancing(shader, type, amount);
	}
}

void Model::drawTreeNormals(Shader shader)
{	
	drawModelNormals(shader);
	for (Model * model : children) {
		model->drawTreeNormals(shader);
	}
}

void Model::drawTreeBoxes(Shader shader)
{
	drawModelBox(shader);
	for (Model * model : children) {
		model-> drawTreeBoxes(shader);
	}
}
