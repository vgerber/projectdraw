#include "pgenerator.h"

Model primitives::generate_rectangle(GLfloat width, GLfloat height, glm::vec4 color = glm::vec4(0.0f))
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

Model primitives::generate_circle(GLfloat radius, GLfloat quality, glm::vec4 color)
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

Model primitives::generate_quad(GLfloat width, GLfloat height, GLfloat depth, glm::vec4 color)
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

Model primitives::generate_hightfield(int width, int length, std::vector<float> data) {
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	int iZMap[6] = {0, 1, 1, 0, 1, 0};
	int iXMap[6] = {0, 0, 1, 0, 1, 1};

	for(int x = 0; x < width-1; x++) {
		for(int z = 0; z < length-1; z++) {

			int offset = vertices.size() / 8;

			//normals
			glm::vec3 normalTri1 = glm::vec3(1.0f, 0.0f, 0.0f);
			glm::vec3 normalTri2 = glm::vec3(1.0f, 0.0f, 0.0f);

			//first triangle
			{
				glm::vec3 triVec1 = glm::vec3(
					x + iXMap[1],
					data[(x + iXMap[1]) + (z + iZMap[1]) * length],
					z + iZMap[1]
				);
				glm::vec3 triVec2 = glm::vec3(
					x + iXMap[2],
					data[(x + iXMap[2]) + (z + iZMap[2]) * length],
					z + iZMap[2]
				);
				glm::vec3 originVert = glm::vec3(x, data[x + z * length], z);

				normalTri1 = glm::normalize(glm::cross((triVec1 - originVert), (triVec2 - originVert)));
			}
			//second triangle
			{
				glm::vec3 triVec1 = glm::vec3(
					x + iXMap[4],
					data[(x + iXMap[4]) + (z + iZMap[4]) * length],
					z + iZMap[4]
				);
				glm::vec3 triVec2 = glm::vec3(
					x + iXMap[5],
					data[(x + iXMap[5]) + (z + iZMap[5]) * length],
					z + iZMap[5]
				);
				glm::vec3 originVert = glm::vec3(x, data[x + z * length], z);

				normalTri2 = glm::normalize(glm::cross((triVec1 - originVert), (triVec2 - originVert)));
			}

			for(int i = 0; i < 6; i++) {
				vertices.push_back(x + iXMap[i]);
				vertices.push_back(data[(x + iXMap[i]) + (z + iZMap[i]) * length]);
				vertices.push_back(z + iZMap[i]);


				glm::vec3 normal = (i < 3) ? normalTri1 : normalTri2;
				

				vertices.push_back(normal.x);
				vertices.push_back(normal.y);
				vertices.push_back(normal.z);

				//texcoords
				vertices.push_back((float)x / width);
				vertices.push_back((float)z / length);

				indices.push_back(offset + i);
			}
		}
	}

	std::vector<Vertex> vertices_vertex;

	for (GLuint i = 0; i < vertices.size(); i += 8) {
		Vertex vertex;
		vertex.Position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
		vertex.Normal = glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
		vertex.TexCoords = glm::vec2(vertices[i + 6], vertices[i + 7]);
		vertex.Color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
		vertices_vertex.push_back(vertex);
	}

	BasicMesh mesh = BasicMesh(vertices_vertex, indices);

	std::vector<BasicMesh> meshes = { mesh };

	return Model(meshes, std::vector<Texture>());
}

Model primitives::generateHeightfieldStep(int width, int length, std::vector<float> data) {
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	int iZMap[30] = { 
		0, 1, 1, 0, 1, 0,
		0, 1, 1, 0, 1, 0, //X Front
		1, 0, 1, 0, 0, 1, //X Back
		0, 0, 0, 0, 0, 0, //Z Front
		1, 1, 1, 1, 1, 1, //Z Back
	};
	int iXMap[30] = { 
		0, 0, 1, 0, 1, 1, 
		0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1,
		1, 0, 1, 0, 0, 1,
		0, 1, 1, 0, 1, 0
	};
	int iYMap[6] = {
		0, 0, 1, 0, 1, 1,
	};


	for (int x = 0; x < width; x++) {
		for (int z = 0; z < length; z++) {

			int offset = vertices.size() / 8;


			float y = data[x + z * length];

			float bordersDelta[4] = {
				0.0f, 0.0f, 0.0f, 0.0f
			};



			/*
			x-> 2
			--|---|---
			0 |   | 1
			  |   |
			--|---|---
			z   3
			|
			v
			*/

			//check height difference for neighbor cells (always go back to zero at edges of field)
			if (x == 0) {
				bordersDelta[0] = -y;
			}
			if (x == width-1) {
				bordersDelta[1] = -y;
			}
			else {
				bordersDelta[1] = data[(x+1) + z * length] - y;
			}
			if (z == 0) {
				bordersDelta[2] = -y;
			}
			if (z == length - 1) {
				bordersDelta[3] = -y;
			}
			else {
				bordersDelta[3] = data[x + (z+1) * length] - y;
			}

			for (int i = 0; i < 4; i++) {
				float deltaHeight = bordersDelta[i];
				float dY = deltaHeight;
				if (deltaHeight == 0.0f) {
					continue;
				}
				offset = vertices.size() / 8;
				deltaHeight += y;
				for (int j = 0; j < 6; j++) {
					vertices.push_back(x + iXMap[(i + 1) * 6 + j]);

					if (iYMap[j] == 0) {
						vertices.push_back(deltaHeight);
					}
					else {
						vertices.push_back(y);
					}
					vertices.push_back(z + iZMap[(i + 1) * 6 + j]);


					glm::vec3 normal = glm::vec3(1.0f);
					if (i == 0) {
						normal = glm::vec3(dY < 0.0f ? -1.0f : 1.0f, 0.0f, 0.0f);
					}
					if (i == 1) {
						normal = glm::vec3(dY < 0.0f ? 1.0f : -1.0f, 0.0f, 0.0f);
					}
					if (i == 2) {
						normal = glm::vec3(0.0f, 0.0f, dY < 0.0f ? -1.0f : 1.0f);
					}
					if (i == 3) {
						normal = glm::vec3(0.0f, 0.0f, dY < 0.0f ? 1.0f : -1.0f);
					}


					vertices.push_back(normal.x);
					vertices.push_back(normal.y);
					vertices.push_back(normal.z);

					//texcoords
					vertices.push_back((float)x / width);
					vertices.push_back((float)z / length);

					indices.push_back(offset + j);
				}
			}
			offset = vertices.size() / 8;
			for (int i = 0; i < 6; i++) {
				vertices.push_back(x + iXMap[i]);
				vertices.push_back(y);
				vertices.push_back(z + iZMap[i]);




				vertices.push_back(0.0f);
				vertices.push_back(1.0f);
				vertices.push_back(0.0f);

				//texcoords
				vertices.push_back((float)x / width);
				vertices.push_back((float)z / length);

				indices.push_back(offset + i);
			}
		}
	}

	std::vector<Vertex> vertices_vertex;

	for (GLuint i = 0; i < vertices.size(); i += 8) {
		Vertex vertex;
		vertex.Position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
		vertex.Normal = glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
		vertex.TexCoords = glm::vec2(vertices[i + 6], vertices[i + 7]);
		vertex.Color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
		vertices_vertex.push_back(vertex);
	}

	BasicMesh mesh = BasicMesh(vertices_vertex, indices);

	std::vector<BasicMesh> meshes = { mesh };

	return Model(meshes, std::vector<Texture>());
}