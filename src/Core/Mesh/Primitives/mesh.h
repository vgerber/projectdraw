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
	/**
	 * @brief Construct a new Mesh object with vertices, indices and textures
	 * 
	 * @param vertices 
	 * @param indices 
	 * @param textures 
	 */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<sTexture> textures);

	/**
	 * @brief Construct a new Mesh object with vertices and referring indices
	 * 
	 * @param vertices 
	 * @param indices 
	 */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	/**
	 * @brief Construct a new Mesh object from mesh file
	 * 
	 * This Constructer will generate mesh data from a given file
	 * Data will be loaded by assimp and can handle some standard files e.g. .obj, .fbx
	 * 
	 * @param path 
	 */
	Mesh(std::string path);

	/**
	 * @brief Construct a new empty mesh
	 * 
	 */
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
	 * @brief Add a line to vertex position
	 * 
	 * @param vertex 
	 */
	void lineTo(Vertex vertex);

	/**
	 * @brief Add a line to vertex position
	 * 
	 * @param position 
	 */
	void lineTo(glm::vec3 position);

	/**
	 * @brief Add a path following the given vertex positions
	 * 
	 * @param vertices 
	 */
	void lineTo(std::vector<Vertex> vertices);

	/**
	 * @brief Add a inependent line
	 * 
	 * @param p1 
	 * @param p2 
	 */
	void line(Vertex p1, Vertex p2);

	/**
	 * @brief Add a independant line
	 * 
	 * @param p1 
	 * @param p2 
	 */
	void line(glm::vec3 p1, glm::vec3 p2);

	/**
	 * @brief Add a single vertex to geometry
	 * 
	 * @param vertex 
	 */
	void addVertex(Vertex vertex);

	/**
	 * @brief Add a single vertex
	 * 
	 * @param position 
	 */
	void addVertex(glm::vec3 position);

	/**
	 * @brief Remove vertex by index
	 * 
	 * @param index 
	 */
	void removeVertex(unsigned int index);

	/**
	 * @brief Get all vertices
	 * 
	 * @return std::vector<Vertex> 
	 */
	std::vector<Vertex> getVertices() const;

	/**
	 * @brief Set the vertices 
	 * 
	 * When this method is used you have to provide a valid indices list
	 * @see Geometry::setIndices
	 * 
	 * @param vertices 
	 */
	void setVertices(std::vector<Vertex> vertices);

	/**
	 * @brief Set the vertex order
	 * 
	 * Order or reuse of vertices can be expressed by index lists
	 * If this method is used all indices must be valid (vertex at index location must exist)
	 * 
	 * @param indices 
	 */
	void setIndices(std::vector<unsigned int> indices);

	/**
	 * @brief Get all vertices indices
	 * 
	 * @return std::vector<GLuint> 
	 */
	std::vector<unsigned int> getIndices() const;

	/**
	 * @brief Remove all vertices
	 * 
	 */
	void clear();
protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

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
	 * @brief Refresh mesh size data
	 * 
	 */
	void reloadSize();

	/**
	 * @brief Notifies all listeners if data has been changed
	 * 
	 */
	virtual void callUpdate() override;
};
