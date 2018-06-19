#pragma once
#include <SOIL.h>
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
	Model(std::vector<BasicMesh> meshes, std::vector<Texture> textures);
	~Model();

	

	Size get_size();

	void draw(Shader shader, DrawType draw_type);
	void draw_normals(Shader shader);
private:
	std::vector<Texture> textures_loaded;
	std::vector<BasicMesh> meshes;
	std::string directory;

	void load_model(std::string path);
	void process_node(aiNode* node, const aiScene* scene);
	BasicMesh process_mesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string typeName);

	

};
