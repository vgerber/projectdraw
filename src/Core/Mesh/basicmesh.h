#pragma once
//#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>

#include "../Shader/shader.h"

#include <vector>

enum DrawType {
	TRIANGLEG,
	POINTG,
	LINEG,
};

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

class BasicMesh
{
public:
	BasicMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<sTexture> textures);
	BasicMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	~BasicMesh();

	void draw(Shader shader, DrawType drawType);
	void drawInstancing(Shader shader, DrawType drawType, int amount);
	void drawNormals(Shader normalShader);
	void add_offset(glm::vec3 offset);

	void reload();
	void dispose();

	void setVertices(std::vector<Vertex> vertices);

	GLuint getVAO();

	std::vector<Vertex> getVertices();
	std::vector<GLuint> getIndices();
	std::vector<sTexture> getTextures();
protected:
	GLuint VAO, VBO, EBO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<sTexture> textures;

	void setupMesh();
};
