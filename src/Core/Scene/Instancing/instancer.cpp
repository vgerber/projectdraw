#include "instancer.h"

Instancer::Instancer(Drawable * drawable, int count)
{
	this->drawable = drawable;
	for (int i = 0; i < count; i++) {
		modelMatrices.push_back(glm::mat4(1));
	}
	glGenBuffers(1, &matrixBuffer);

	resize();
}

void Instancer::draw(Shader shader) {
	glUniform1f(glGetUniformLocation(shader.getId(), "useLight"), 1.0f);
	glUniform1i(glGetUniformLocation(shader.getId(), "enableCustomColor"), 0);
	drawable->drawInstancing(shader, DrawType::TRIANGLEG, modelMatrices.size());
}

glm::mat4 Instancer::getModelMatrix(int index)
{
	return modelMatrices[index];
}

std::vector<glm::mat4> Instancer::getModelMatrices()
{
	return modelMatrices;
}

void Instancer::setModelMatrix(glm::mat4 model, int index)
{
	modelMatrices[index] = model;
	resize();
}

void Instancer::setModelMatrices(std::vector<glm::mat4> models) {
	modelMatrices = models;
	resize();
}

void Instancer::dispose()
{
	glDeleteBuffers(1, &matrixBuffer);
}

void Instancer::resize() {
	/*
	glBindBuffer(GL_ARRAY_BUFFER, matrixBuffer);
 	glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	std::vector<Mesh*> meshes = model->getMeshes();
	for (size_t i = 0; i < meshes.size(); i++) {
		Mesh * mesh = meshes[i];
		GLuint VAO = mesh->getVAO();
		glBindVertexArray(VAO);

		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);

		glBindVertexArray(0);
	}
	*/
}