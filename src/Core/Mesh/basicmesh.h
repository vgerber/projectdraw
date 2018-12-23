#pragma once
//#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>

#include "../Shader/shader.h"
#include "Core/Model/model.h"

#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec4 Color = glm::vec4(0.0f);
};

struct sTexture {
	GLuint id;
	std::string type;
	aiString path;
};

struct ColorTexture {
	glm::vec4 Color;
	std::string Type;
};

class Mesh : public Model
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<sTexture> textures);
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	Mesh(std::string path);
	Mesh();
	
	///modifies all vertices with the offset
	void applyMeshOffset(glm::vec3 offset);

	///modifies all vertices with the transformation
	void applyMeshTransformation(Transform transform);

	void applyMeshRecenter(glm::vec3 centerPoint);

	virtual Size getSize() override;

	void dispose();

	void setVertices(std::vector<Vertex> vertices);
	
	std::vector<Vertex> getVertices();
	std::vector<GLuint> getIndices();
	std::vector<sTexture> getTextures();
protected:
	GLuint VAO, VBO, EBO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<sTexture> textures;

	virtual void drawModel(Shader shader, DrawType drawType)					   override;
	virtual void drawModelInstancing(Shader shader, DrawType drawType, int amount) override;
	virtual void drawModelNormals(Shader normalShader)							   override;
	virtual void drawModelBox(Shader boxShader)									   override;

	///load mesh data from file
	void loadMesh(std::string path);

	///process assimp node
	void processNode(aiNode* node, const aiScene* scene);

	///extract mesh from node
	void processMesh(aiMesh* mesh, const aiScene* scene);


	//std::vector<sTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	
	///set parent mesh
	virtual void setParent(Model * parent) override;

	void setupMesh();

	void reloadMeshData();

	void reloadSize();
};
