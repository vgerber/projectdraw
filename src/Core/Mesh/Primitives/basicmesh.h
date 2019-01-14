#pragma once
//#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Shader/shader.h"
#include "Core/Scene/drawable.h"

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

class Mesh : public Drawable
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

	virtual void draw(DrawType drawType = DrawType::TRIANGLEG)					     override;
	virtual void drawInstancing(int amount, DrawType drawType = DrawType::TRIANGLEG) override;

	///load mesh data from file
	void loadMesh(std::string path);

	///process assimp node
	void processNode(aiNode* node, const aiScene* scene);

	///extract mesh from node
	void processMesh(aiMesh* mesh, const aiScene* scene);

	///remove empty mesh nodes and restructre children
	void collapseEmptyMeshes();

	void setupMesh();

	void reloadMeshData();

	void reloadSize();
};
