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
	
	/**
	 * @brief Applies a offset vector to all vertices
	 * 
	 * @param offset 
	 */
	void applyMeshOffset(glm::vec3 offset);

	/**
	 * @brief Applies a transformation to all vertices
	 * 
	 * @param transform 
	 */
	void applyMeshTransformation(Transform transform);

	/**
	 * @brief Transform mesh to new mesh center
	 * 
	 * @param centerPoint 
	 */
	void applyMeshRecenter(glm::vec3 centerPoint);

	/**
	 * @brief Get the bounding size of the mesh
	 * 
	 * @return Size 
	 */
	virtual Size getSize() override;

	/**
	 * @brief Free all resources
	 * 
	 */
	void dispose();

	/**
	 * @brief Set the vertices vector
	 * 
	 * @param vertices 
	 */
	void setVertices(std::vector<Vertex> vertices);
	
	/**
	 * @brief Get all mesh vertices
	 * 
	 * @return std::vector<Vertex> 
	 */
	std::vector<Vertex> getVertices();

	/**
	 * @brief Get all vertices indices
	 * 
	 * @return std::vector<GLuint> 
	 */
	std::vector<GLuint> getIndices();
	std::vector<sTexture> getTextures();
protected:
	GLuint VAO, VBO, EBO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<sTexture> textures;

	virtual void draw(DrawType drawType = DrawType::TRIANGLEG) override;

	/**
	 * @brief Load data from file
	 * 
	 * @param path 
	 */
	void loadMesh(std::string path);

	/**
	 * @brief Processes node if current node has subnodes
	 * 
	 * @param node 
	 * @param scene 
	 */
	void processNode(aiNode* node, const aiScene* scene);

	/**
	 * @brief Read and save mesh data
	 * 
	 * @param mesh 
	 * @param scene 
	 */
	void processMesh(aiMesh* mesh, const aiScene* scene);

	/**
	 * @brief remove emtpy nodes
	 * 
	 */
	void collapseEmptyMeshes();

	/**
	 * @brief Setup memory data
	 * 
	 */
	void setupMesh();

	/**
	 * @brief Refresh buffer data
	 * 
	 */
	void reloadMeshData();

	/**
	 * @brief Refresh mesh size data
	 * 
	 */
	void reloadSize();
};
