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

	///free all gl resources
	virtual void dispose() = 0;

	///draw object with all settings
	virtual void draw() = 0;

	///draw plain object geometry
	virtual void drawRaw() = 0;

	///draw normals for each vertex
	virtual void drawNormals(Shader shader) = 0;

	///draw bounding box (not aabb) and rotation center
	virtual void drawBox(Shader shader) = 0;

	///set mesh shader
	///mesh shader will be set by the renderer
	void setShader(Shader shader, AbstractRenderer &renderer);

	///get mesh shader
	std::pair<Shader, int> getShader();

	///set view and camera matrix for mvp calculation
	virtual void setCameraMatrices(glm::mat4 cView, glm::mat4 cProj);		

protected:
	glm::mat4 cameraView = glm::mat4(1.0);
	glm::mat4 cameraProj = glm::mat4(1.0);
	int currentRenderer;

	//shader for drawing the mesh
	Shader shader;
};
