#pragma once

#include "sceneobject.h"
#include "../Model/model.h"
#include "../Model/box.h"
#include "Renderer/arenderer.h"

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
	DrawType xrayDrawType = DrawType::TRIANGLEG;

	//Outline
	bool outlineVisible = false;
	float outlineThickness = 1.0f;
	glm::vec4 outlineColor = glm::vec4(1.0f);
};

class Drawable : public SceneObject
{
public:
	Drawable();
	~Drawable();


	DrawableInfo settings;

	//free all gl resources
	virtual void dispose();

	//draw object with all settings
	virtual void draw();

	//draw plain object geometry
	virtual void drawRaw();

	//draw normals for each vertex
	virtual void drawNormals(Shader shader);

	//draw bounding box (not aabb) and rotation center
	virtual void drawBox(Shader shader);

	//set model for drawbale
	virtual void setModel(Model model);

	//set mesh shader
	void setShader(Shader shader, AbstractRenderer &renderer);
	std::pair<Shader, int> getShader();

	
	Size getAABBBox();	
	
	void setCenter(glm::vec3 center) override;
	void setCenterInWorld(glm::vec3 point) override;

	//set view and camera matrix for mvp calculation
	void setCameraMatrices(glm::mat4 cView, glm::mat4 cProj);

	Model getModel();
	Model* getModelPtr();
	
	unsigned int getDimension();

protected:
	glm::mat4 cameraView = glm::mat4(1.0);
	glm::mat4 cameraProj = glm::mat4(1.0);
	int currentRenderer;
	GLuint boxVBO, boxVAO;
	Model objModel;

	//shader for drawing the mesh
	Shader shader;

	virtual void setup();
	
	virtual void loadBox();
private:
	unsigned int dimension = 0;
};
