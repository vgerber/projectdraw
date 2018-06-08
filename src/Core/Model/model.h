#include <SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "../Mesh/basicmesh.h"

class Model
{
public:
	Model();
	Model(GLchar* path);
	Model(std::vector<BasicMesh> meshes, std::vector<Texture> textures);
	~Model();

	void Draw(Shader shader);
	void DrawNormals(Shader shader);
private:
	std::vector<Texture> textures_loaded;
	std::vector<BasicMesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	BasicMesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};
