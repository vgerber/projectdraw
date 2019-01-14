#pragma once

#include "Core/Scene/sceneobject.h"
#include "Core/Shader/shader.h"
#include "Core/Scene/Renderer/arenderer.h"

#include <memory>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

enum DrawType {
	TRIANGLEG,
	POINTG,
	LINEG,
};

struct DrawableInfo {
	//Drawing
	DrawType drawType = DrawType::TRIANGLEG;
	bool useLight = true;
	bool useCustomColor = false;
	glm::vec4 customColor = glm::vec4(1.0f);

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
	DrawType xrayDrawType = DrawType::TRIANGLEG;

	//Outline
	bool outlineVisible = false;
	float outlineThickness = 1.0f;
	glm::vec4 outlineColor = glm::vec4(1.0f);
};


class Drawable : public SceneObject
{
public:

	DrawableInfo settings;

	virtual Size getSize() = 0;

	///Draw model with drawing settings
	virtual void draw(DrawType drawType = DrawType::TRIANGLEG) = 0;

	///Draw with instancing
	virtual void drawInstancing(int amount, DrawType drawType = DrawType::TRIANGLEG) = 0;

	///Draw bounding box with model center (not aabb)
	virtual void drawBox();

	///Set camera matrices and generate mvp matrix
	///Used by renderer (for caching mvp)
	virtual void setCameraMatrices(glm::mat4 cView, glm::mat4 cProj);

	///get cached mvp matrix
	glm::mat4 getMVPMatrix();

	///Destroy aquired resources
	void dispose();	

protected:
	glm::mat4 mvp = glm::mat4(1.0);
private:
};
