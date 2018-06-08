#include "cube.h"


Cube::Cube(float x, float y, float z, float width, float length, float height, glm::vec4 color = glm::vec4(0.0f))
{
	std::vector<GLfloat> vertices = {
		x,			y,			z,			0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		x + width,	y,			z,			0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 
		x + width,  y + length, z,			0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 
		x,			y + length, z,			0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 

		x,			y,			z + height, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
		x + width,	y,			z + height, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 
		x + width,	y + length,	z + height, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 
		x,			y + length, z + height, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 

		x,			y + length,	z + height, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
		x,			y + length, z,			-1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
		x,			y,			z,			-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 
		x,			y,			z + height, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 

		x + width,  y + length, z + height, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
		x + width,  y + length, z,			1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
		x + width,	y,			z,			1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 
		x + width,	y,			z + height, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 

		x,			y,			z,			0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 
		x + width,	y,			z,			0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 
		x + width,	y,			z + height, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 
		x,			y,			z + height, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 

		x,			y + length, z,			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 
		x + width,  y + length, z,			0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 
		x + width,  y + length, z + height, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
		x,			y + length, z + height, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
	};


	std::vector<GLuint> indices = {
		//back
		0, 3, 2, 
		2, 1, 0,
		//front
		4, 5, 6,
		6, 7, 4,
		//Left
		14, 13, 12,
		12, 15, 14,
		//Right
		8, 9, 10, 
		10, 11, 8,
		//bottom
		16, 17, 18,
		18, 19, 16,
		//top
		20, 23, 22,
		22, 21, 20
	};

	std::vector<Vertex> vertices_vertex;

	for (GLuint i = 0; i < vertices.size(); i += 8) {
		Vertex vertex;
		vertex.Position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
		vertex.Normal = glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
		vertex.TexCoords = glm::vec2(0.0f);//glm::vec2(vertices[i + 6], vertices[i + 7]);
		vertex.Color = color;
		vertices_vertex.push_back(vertex);
	}

	BasicMesh mesh = BasicMesh(vertices_vertex, indices);

	std::vector<BasicMesh> meshes = {mesh};

	objModel = Model(meshes, std::vector<Texture>());
}

Cube::~Cube()
{
}
