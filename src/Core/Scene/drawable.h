#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <btBulletDynamicsCommon.h>

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

class Drawable : public SceneObject, public BoundingBox
{
public:
	Drawable();
	Drawable(GLchar* mmodel);
	~Drawable();


	DrawableInfo dInfo;

	glm::mat4 mmodel = glm::mat4(1.0f);

	virtual void dispose();
	virtual void draw();
	virtual void draw(Shader shader);
	virtual void drawNormals();
	virtual void drawNormals(Shader shader);
	virtual void drawBox();
	virtual void drawBox(Shader shader);
	

	glm::mat4 getModelMatrix();

	void rotate(GLfloat x, GLfloat y, GLfloat z);
	void rotate(glm::vec3 vrotation);
	void translate(GLfloat x, GLfloat y, GLfloat z);
	void translate(glm::vec3 vtranslation);
	void scale(GLfloat x, GLfloat y, GLfloat z);
	void scale(glm::vec3 vscale);

	Size getSize();
	Size getAABBBox();
	void scaleToSize(Size size);
	void scaleToWidth(GLfloat width);
	void scaleToHeight(GLfloat height);
	void scaleToLength(GLfloat depth);

	glm::vec3 getPosition();
	glm::vec3 getPositionCenter();

	glm::vec3 getScale();
	glm::vec3 getRotation();	

	void setPosition(glm::vec3 position);
	void setPositionCenter(glm::vec3 position);

	void setCenter(glm::vec3 center);
	void setCenterInWorld(glm::vec3 point);

	glm::vec3 getCenter();
	glm::vec3 getCenterPoint();

	void transform(const btTransform &transform);


	Model getModel();
	Model* getModelInstance();
	void setModel(Model model);

	void setShader(Shader shader);
	unsigned int getDimension();
protected:
	GLuint boxVBO, boxVAO;
	Size size;
	Model objModel;
	Shader shader;
	Shader shader_normals;
	Shader shader_box;

	glm::vec3 vcenter = glm::vec3(0.5f);
	glm::vec3 vrotation = glm::vec3(0.0f);
	glm::vec3 vscale = glm::vec3(1.0f);
	

	virtual void setup();
	virtual void updateModel();
	virtual void loadBox();
private:
	bool size_changed = true;
	unsigned int dimension = 0;
};
