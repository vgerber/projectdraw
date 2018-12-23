#pragma once
//#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>


#include "Core/Mesh/basicmesh.h"

class Instancer {
public:
	Instancer(Model * model, int count);

	void draw(Shader shader);

	glm::mat4 getModelMatrix(int index);
	std::vector<glm::mat4> getModelMatrices();
	void setModelMatrix(glm::mat4 model, int index);
	void setModelMatrices(std::vector<glm::mat4> models);

	void dispose();
private:
	GLuint matrixBuffer;
	Model * model;
	std::vector<glm::mat4> modelMatrices;
	Shader shader;

	void resize();
};