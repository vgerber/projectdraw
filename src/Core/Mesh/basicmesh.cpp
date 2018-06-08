#include "basicmesh.h"


BasicMesh::BasicMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

BasicMesh::BasicMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
	this->vertices = vertices;
	this->indices = indices;

	setupMesh();
}

BasicMesh::~BasicMesh()
{
}

void BasicMesh::Draw(Shader shader)
{
	//glUseProgram(shader.GetProgrammId());
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
						
		std::string name = this->textures[i].type;
		std::string number;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++); // Transfer GLuint to stream
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // Transfer GLuint to stream
		// Now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.GetProgrammId(), (name + number).c_str()), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}

	// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
	//glUniform1f(glGetUniformLocation(shader.GetProgrammId(), "material.shininess"), 16.0f);

	// Draw mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

	// Always good practice to set everything back to defaults once configured.
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void BasicMesh::DrawNormals(Shader normalShader)
{
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

std::vector<Vertex> BasicMesh::GetVertices()
{
	return vertices;
}

std::vector<GLuint> BasicMesh::GetIndices()
{
	return indices;
}

std::vector<Texture> BasicMesh::getTextures()
{
	return textures;
}

void BasicMesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));
	glBindVertexArray(0);
}
