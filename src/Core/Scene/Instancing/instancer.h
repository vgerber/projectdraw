#pragma once
//#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>


#include "Core/Mesh/Primitives/mesh.h"

class Instancer {
public:
	Instancer(Drawable * drawable, int count);

	void draw(DrawType drawType = DrawType::TRIANGLEG);

	glm::mat4 getModelMatrix(int index);
	std::vector<glm::mat4> getModelMatrices();
	void setModelMatrix(glm::mat4 model, int index);
	void setModelMatrices(std::vector<glm::mat4> models);

	void dispose();
private:
	GLuint matrixBuffer;
	Drawable * drawable;
	std::vector<glm::mat4> modelMatrices;
	Shader shader;

	void resize();
};