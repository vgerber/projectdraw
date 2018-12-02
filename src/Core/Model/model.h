#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "../Mesh/basicmesh.h"
#include "box.h"

class Model
{
public:
	Model();
	Model(GLchar* path);
	Model(std::vector<BasicMesh> meshes, std::vector<sTexture> textures);
	~Model();

	void setMesh(int meshIndex, BasicMesh mesh);

	Size getSize();
	std::vector<BasicMesh> getMeshes();

	void draw(Shader shader, DrawType drawType);
	void drawInstancing(Shader shader, DrawType drawType, int amount);
	void drawNormals(Shader shader);
	void dispose();
private:
	std::vector<Model> subModels;

	glm::mat4 baseTransform;
	std::vector<sTexture> textures_loaded;
	std::vector<BasicMesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	BasicMesh process_mesh(aiMesh* mesh, const aiScene* scene);
	std::vector<sTexture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string typeName);

	

};
