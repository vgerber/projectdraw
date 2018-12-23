#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <sstream>
#include <string>

#include "Core/Scene/drawable.h"
#include "box.h"

class Model : public Drawable
{
public:
	virtual Size getSize() = 0;

	/*
	std::vector<Model*> getChildren();
	Model * getChildModel(int index);
	Model * getModel(std::string name);
	*/

	///Get transform from model
	Transform getModelBaseTransform();

	///Get accumulated transform from root model to current model
	Transform getTreeBaseTransfrom();

	///Apply transform to current model
	void applyBaseTransform(Transform transform);

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

	///Destroy aquired resources
	void dispose();
protected:
	std::vector<Model*> children;
	std::string modelName = "";
	Transform baseTransform;
	
	Model * parent = nullptr;
	std::string directory;

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

	///set parent model
	virtual void setParent(Model * model);
};
