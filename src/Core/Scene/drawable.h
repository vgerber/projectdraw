#pragma once

#include "sceneobject.h"
#include "../Model/model.h"
#include "../Model/box.h"
#include <memory>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

struct DrawableInfo {
	//Drawing
	DrawType drawType = DrawType::TRIANGLEG;

	//Normals
	bool normalVisible = false;
	float normalSize = 1.0f;
	float normalThickness = 1.0f;
	glm::vec4 normalColor = glm::vec4(1.0f);
	
	//Box
	bool boxVisible = false;
	float boxThickness = 1.0f;
	glm::vec4 boxColor = glm::vec4(1.0f);

	//XRay
	bool xrayVisible = false;
	bool xrayUseLight = false;
	bool xrayCustomColor = true;
	glm::vec4 xrayColor = glm::vec4(1.0f);

	//Outline
	bool outlineVisible = false;
	float outlineThickness = 1.0f;
	glm::vec4 outlineColor = glm::vec4(1.0f);
};

class Drawable : public SceneObject
{
public:
	Drawable();
	Drawable(GLchar* mmodel);
	~Drawable();


	DrawableInfo settings;


	virtual void dispose();
	virtual void draw();
	virtual void draw(Shader shader);
	virtual void drawNormals();
	virtual void drawNormals(Shader shader);
	virtual void drawBox();
	virtual void drawBox(Shader shader);
	virtual void setModel(Model model);
	virtual void setShader(Shader shader);

	
	Size getAABBBox();	
	
	void setCenter(glm::vec3 center) override;
	void setCenterInWorld(glm::vec3 point) override;


	Model getModel();
	Model* getModelPtr();
	
	unsigned int getDimension();
protected:
	GLuint boxVBO, boxVAO;
	Model objModel;
	Shader shader;
	Shader shader_normals;
	Shader shader_box;

	


	virtual void setup();
	
	virtual void loadBox();
private:
	unsigned int dimension = 0;
};
