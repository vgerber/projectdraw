#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>

#include "../Shader/shader.h"

#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec4 Color = glm::vec4(0.0f);
};

struct Texture {
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
	BasicMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	BasicMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	~BasicMesh();

	void draw(Shader shader);
	void draw_normals(Shader normalShader);

	std::vector<Vertex> get_vertices();
	std::vector<GLuint> get_indices();
	std::vector<Texture> get_textures();
protected:
	GLuint VAO, VBO, EBO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	void setupMesh();
};
