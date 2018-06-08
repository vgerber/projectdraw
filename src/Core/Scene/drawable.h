#pragma once

#include "sceneobject.h"
#include "../Model/model.h"

#include <memory>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

class Drawable : public SceneObject
{
public:
	Drawable();
	Drawable(GLchar* model);
	~Drawable();

	virtual void Dispose();
	virtual void Draw(Shader shader);
	glm::mat4 GetModelMatrix();

	void Rotate(GLfloat x, GLfloat y, GLfloat z);
	void Rotate(glm::vec3 rotation);
	void Translate(GLfloat x, GLfloat y, GLfloat z);
	void Translate(glm::vec3 translation);
	void Scale(GLfloat x, GLfloat y, GLfloat z);
	void Scale(glm::vec3 scale);

	glm::vec3 GetPosition();
	glm::vec3 GetScale();
	glm::vec3 GetRotion();
	void SetPosition(glm::vec3 position);

protected:
	Shader normalShader;
	Model objModel;

	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::vec3 translation = glm::vec3(0.0f);
	glm::mat4 model = glm::mat4(1.0f);

	void updateModel();
};
