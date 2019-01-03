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
	virtual void draw();

	///Draw model as plain geometry (without features)
	virtual void drawRaw();

	///Draw with instancing
	virtual void drawInstancing(Shader shader, DrawType drawType, int amount);

	///Draw model normals
	virtual void drawNormals(Shader shader);

	///Draw bounding box with model center (not aabb)
	virtual void drawBox(Shader shader);

	///Set camera matrices
	virtual void setCameraMatrices(glm::mat4 cView, glm::mat4 cProj);

	///Set shader for model
	///shader will be set by renderer
	virtual void setShader(Shader shader, AbstractRenderer &renderer);

	///get mesh shader
	std::pair<Shader, int> getShader();

	///Destroy aquired resources
	void dispose();	

protected:
	glm::mat4 cameraView = glm::mat4(1.0);
	glm::mat4 cameraProj = glm::mat4(1.0);
	int currentRenderer;

	///shader for drawing the mesh
	Shader shader;

	///draw meshes + submeshes
	virtual void drawTree(Shader shader, DrawType type);

	///instancing  meshes + submeshes
	virtual void drawTreeInstancing(Shader shader, DrawType type, int amount);

	///draw mesh normals + submesh normals
	virtual void drawTreeNormals(Shader shader);

	///draw mesh normals + submesh normals
	virtual void drawTreeBoxes(Shader shader);

	///draw meshes + submeshes
	virtual void drawModel(Shader shader, DrawType type) = 0;

	///instancing  meshes + submeshes
	virtual void drawModelInstancing(Shader shader, DrawType type, int amount) = 0;

	///draw mesh normals + submesh normals
	virtual void drawModelNormals(Shader shader) = 0;

	///draw mesh bounding box
	virtual void drawModelBox(Shader shader) = 0;
private:
};
