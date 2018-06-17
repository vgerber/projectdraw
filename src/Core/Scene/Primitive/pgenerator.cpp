#include "pgenerator.h"

Model primitves::generate_rectangle(GLfloat width, GLfloat height, glm::vec4 color = glm::vec4(0.0f))
{
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f;
	std::vector<GLfloat> vertices = {
		x,			y,			z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		x + width,	y,			z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		x + width,  y + height, z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		x,			y + height, z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

		x,			y,			z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		x + width,	y,			z, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		x + width,	y + height,	z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		x,			y + height, z, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
	};


	std::vector<GLuint> indices = {
		//back
		0, 3, 2,
		2, 1, 0,
		//front
		4, 5, 6,
		6, 7, 4,
	};

	std::vector<Vertex> vertices_vertex;

	for (GLuint i = 0; i < vertices.size(); i += 8) {
		Vertex vertex;
		vertex.Position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
		vertex.Normal = glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
		vertex.TexCoords = glm::vec2(0.0f);
		vertex.Color = color;
		vertices_vertex.push_back(vertex);
	}

	BasicMesh mesh = BasicMesh(vertices_vertex, indices);

	std::vector<BasicMesh> meshes = { mesh };

	return Model(meshes, std::vector<Texture>());
}

Model primitves::generate_circle(GLfloat radius, GLfloat quality, glm::vec4 color)
{
	if (quality < 3.0f) {
		quality = 3.0f;
	}
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f;

	GLfloat step = 2 * glm::pi<GLfloat>() / quality;

	std::vector<GLfloat> vertices;

	

	for (GLfloat theta = 0; theta < 2 * glm::pi<GLfloat>(); theta += step) {
		GLfloat x_tex = cos(theta);
		GLfloat y_tex = sin(theta);
		GLfloat x = x_tex * radius;
		GLfloat y = y_tex * radius;

		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(1.0f);
		vertices.push_back(x_tex * 0.5f + 0.5f);
		vertices.push_back(y_tex * 0.5f + 0.5f);
	}
	size_t size_back = vertices.size();

	for (size_t i = 0; i < size_back; i += 8) {
		vertices.push_back(vertices[i]);
		vertices.push_back(vertices[i + 1]);
		vertices.push_back(vertices[i + 2]);
		vertices.push_back(vertices[i + 3]);
		vertices.push_back(vertices[i + 4]);
		vertices.push_back(vertices[i + 5] * -1.0f);
		vertices.push_back(vertices[i + 6]);
		vertices.push_back(vertices[i + 7]);
	}

	size_back = vertices.size() / 16;

	std::vector<GLuint> indices;

	for (size_t i = 1; i < size_back-1; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
		
		indices.push_back(size_back);
		indices.push_back(size_back + i + 1);
		indices.push_back(size_back + i);
		
	}

	std::vector<Vertex> vertices_vertex;

	for (GLuint i = 0; i < vertices.size(); i += 8) {
		Vertex vertex;
		vertex.Position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
		vertex.Normal = glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
		vertex.TexCoords = glm::vec2(0.0f);
		vertex.Color = color;
		vertices_vertex.push_back(vertex);
	}

	BasicMesh mesh = BasicMesh(vertices_vertex, indices);

	std::vector<BasicMesh> meshes = { mesh };

	return Model(meshes, std::vector<Texture>());
}

Model primitves::generate_quad(GLfloat width, GLfloat height, GLfloat depth, glm::vec4 color)
{
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f;
	std::vector<GLfloat> vertices = {
		x,			y,			z,			0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		x + width,	y,			z,			0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		x + width,  y + height, z,			0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		x,			y + height, z,			0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

		x,			y,			z + depth, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		x + width,	y,			z + depth, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		x + width,	y + height,	z + depth, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		x,			y + height, z + depth, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

		x,			y + height,	z + depth, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		x,			y + height, z,			-1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		x,			y,			z,			-1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		x,			y,			z + depth, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		x + width,  y + height, z + depth, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		x + width,  y + height, z,			1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		x + width,	y,			z,			1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		x + width,	y,			z + depth, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		x,			y,			z,			0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		x + width,	y,			z,			0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		x + width,	y,			z + depth, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		x,			y,			z + depth, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

		x,			y + height, z,			0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		x + width,  y + height, z,			0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		x + width,  y + height, z + depth, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		x,			y + height, z + depth, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
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

	std::vector<BasicMesh> meshes = { mesh };

	return Model(meshes, std::vector<Texture>());
}
