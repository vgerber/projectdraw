#include "basicmesh.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<sTexture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
	reloadSize();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
	this->vertices = vertices;
	this->indices = indices;

	setupMesh();
	reloadSize();
}

Mesh::Mesh(std::string path)
{
	loadMesh(path);
	setupMesh();
	reloadSize();
}

Mesh::Mesh()
{
	setupMesh();
}

void Mesh::applyMeshOffset(glm::vec3 offset)
{
	for (size_t i = 0; i < vertices.size(); i++) {
		vertices[i].Position += offset;
	}
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
	reloadSize();
}

void Mesh::applyMeshTransformation(Transform transfrom) {
	for (size_t i = 0; i < vertices.size(); i++) {
		vertices[i].Position = glm::vec3(transfrom.getMatrix() * glm::vec4(vertices[i].Position, 1.0));
	}
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
	reloadSize();
}

void Mesh::applyMeshRecenter(glm::vec3 centerPoint)
{	
	glm::vec3 currentCenter(
		size.x + 0.5f * size.width,
		size.y + 0.5f * size.height,
		size.z + 0.5f * size.depth
	);
	applyMeshOffset(centerPoint - currentCenter);
}

Size Mesh::getSize()
{
	return size;
}

void Mesh::dispose() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	for (auto child : children) {
		child->dispose();
	}
}

void Mesh::setVertices(std::vector<Vertex> vertices)
{
	this->vertices = vertices;
}

std::vector<Vertex> Mesh::getVertices()
{
	return vertices;
}

std::vector<GLuint> Mesh::getIndices()
{
	return indices;
}

std::vector<sTexture> Mesh::getTextures()
{
	return textures;
}

void Mesh::draw(DrawType drawType)
{
	if (vertices.size() == 0) {
		return;
	}
	/*
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
		glUniform1i(glGetUniformLocation(shader.getId(), (name + number).c_str()), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}
	*/
	// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
	//glUniform1f(glGetUniformLocation(shader.GetProgrammId(), "material.shininess"), 16.0f);

	// Draw mesh
	glBindVertexArray(this->VAO);
	if (drawType == DrawType::LINEG) {
		glDrawElements(GL_LINE_STRIP, this->indices.size(), GL_UNSIGNED_INT, 0);
	}
	else if (drawType == DrawType::POINTG) {
		glDrawElements(GL_POINTS, this->indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	}

	// Always good practice to set everything back to defaults once configured.
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

/*
void Mesh::drawInstancing(int amount, DrawType drawType)
{
	glBindVertexArray(this->VAO);
	if (drawType == DrawType::LINEG) {
		glDrawElementsInstanced(GL_LINE_STRIP, this->indices.size(), GL_UNSIGNED_INT, 0, amount);
	}
	else if (drawType == DrawType::POINTG) {
		glDrawElementsInstanced(GL_POINTS, this->indices.size(), GL_UNSIGNED_INT, 0, amount);
	}
	else {
		glDrawElementsInstanced(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0, amount);
	}
}
*/

void Mesh::loadMesh(std::string path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	this->processNode(scene->mRootNode, scene);
}

void Mesh::processNode(aiNode * node, const aiScene * scene)
{
	id = std::string(node->mName.C_Str());
	if (id.size() == 0) {
		std::stringstream ss;
		ss << this;
		id = ss.str();
	}
	aiMatrix4x4 transform = node->mTransformation;

	glm::mat4 baseTransformMatrix = glm::mat4(0.0f);
	baseTransformMatrix[0][0] = transform.a1;
	baseTransformMatrix[1][0] = transform.a2;
	baseTransformMatrix[2][0] = transform.a3;
	baseTransformMatrix[3][0] = transform.a4;

	baseTransformMatrix[0][1] = transform.b1;
	baseTransformMatrix[1][1] = transform.b2;
	baseTransformMatrix[2][1] = transform.b3;
	baseTransformMatrix[3][1] = transform.b4;

	baseTransformMatrix[0][2] = transform.c1;
	baseTransformMatrix[1][2] = transform.c2;
	baseTransformMatrix[2][2] = transform.c3;
	baseTransformMatrix[3][2] = transform.c4;

	baseTransformMatrix[0][3] = transform.d1;
	baseTransformMatrix[1][3] = transform.d2;
	baseTransformMatrix[2][3] = transform.d3;
	baseTransformMatrix[3][3] = transform.d4;

	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 scale;
	glm::vec3 skew;
	glm::vec4 persp;
	glm::decompose(baseTransformMatrix, scale, rotation, translation, skew, persp);
	Transform baseTransform;
	translate(translation);
	//baseTransform.translate(translation);
	baseTransform.rotate(Rotator(rotation, glm::vec3(0.0)));
	baseTransform.scale(scale);
	//baseTransform.print();

	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
		applyMeshTransformation(parentTransform * baseTransform);
	}
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		Mesh * childMesh = new Mesh();
		childMesh->parentTransform = baseTransform * this->parentTransform;
		childMesh->processNode(node->mChildren[i], scene);		
		children.push_back(childMesh);
	}
	//clear transformation nodes
	collapseEmptyMeshes();
	//transformChanged();
	reloadMeshData();
	reloadSize();
}

void Mesh::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<sTexture> textures;

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	glm::vec4 color = glm::vec4(0.0f);
	aiColor4D diffuse;
	if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
		color = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	}
	//load vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 tmp;
		tmp.x = mesh->mVertices[i].x;
		tmp.y = mesh->mVertices[i].y;
		tmp.z = mesh->mVertices[i].z;
		vertex.Position = tmp;

		tmp.x = mesh->mNormals[i].x;
		tmp.y = mesh->mNormals[i].y;
		tmp.z = mesh->mNormals[i].z;
		vertex.Normal = tmp;

		if (mesh->mTextureCoords[0]) {
			glm::vec2 tmp;
			tmp.x = mesh->mTextureCoords[0][i].x;
			tmp.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = tmp;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertex.Color = color;

		vertices.push_back(vertex);
	}
	//load indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	//load material
	/*
	if (mesh->mMaterialIndex >= 0) {
		material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<sTexture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.begin(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<sTexture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.begin(), specularMaps.begin(), specularMaps.end());
	}
	*/
	this->vertices = vertices;
	this->indices = indices;
	//return new Mesh(vertices, indices, textures);
}

void Mesh::collapseEmptyMeshes() {
	for(size_t i = 0; i < children.size(); i++) {
		Mesh * childMesh = static_cast<Mesh*>(children[i]);
		childMesh->collapseEmptyMeshes();
		if(childMesh->vertices.size() == 0) {
			for(auto child : childMesh->children) {
				//move children of childMesh to new parent (this)
				child->setTransform(childMesh->getTransform() * child->getTransform());
				this->children.push_back(child);
			}
			childMesh->children.clear();
			childMesh->dispose();
			this->children.erase(this->children.begin() + i);
			delete childMesh;
			i--;
		}
	}
	//update parent transforms
	transformChanged();
}

/*
std::vector<sTexture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<sTexture> textures;
	aiColor3D color;
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		//check if already loaded
		GLboolean loaded = false;
		for (GLuint j = 0; j < textures_loaded.size(); j++) {
			if (textures_loaded[j].path == str) {
				textures.push_back(textures_loaded[j]);
				loaded = true;
				break;
			}
		}
		if (!loaded) {
			sTexture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			//add to loaded textures
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}
*/ 

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	reloadMeshData();
}

void Mesh::reloadMeshData()
{
	if (vertices.size() > 0 && indices.size() > 0) {
		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
	
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));
		glBindVertexArray(0);
	}
}

void Mesh::reloadSize()
{
	Size size;
	bool initRequired = true;
	if (children.size() >= 0) {
		for (auto child : children) {
			Size childSize = child->getSize();
			if (initRequired) {
				size.x = childSize.x;
				size.y = childSize.y;
				size.z = childSize.z;
				size.width = childSize.width;
				size.height = childSize.height;
				size.depth = childSize.depth;
				initRequired = false;
				continue;
			}
			//min x
			if (size.x < childSize.x) {
				size.x = childSize.x;
			}
			//max width
			if (size.width > childSize.width) {
				size.width = childSize.width;
			}
			//min y
			if (size.y < childSize.y) {
				size.y = childSize.y;
			}
			//max height
			if (size.height > childSize.height) {
				size.height = childSize.height;
			}
			//min z
			if (size.z < childSize.z) {
				size.z = childSize.z;
			}
			//max length
			if (size.depth > childSize.depth) {
				size.depth = childSize.depth;
			}
		}
	}
	initRequired = size.width > 0.0f || size.height > 0.0f || size.depth > 0.0f;
	GLfloat x, y, z;

	for (Vertex vertex : vertices) {
		x = vertex.Position.x;
		y = vertex.Position.y;
		z = vertex.Position.z;
		if (initRequired) {
			size.x = x;
			size.y = y;
			size.z = z;
			size.width = x;
			size.height = y;
			size.depth = z;
			initRequired = false;
			continue;
		}
		//min x
		if (x < size.x) {
			size.x = x;
		}
		//max width
		if (x > size.width) {
			size.width = x;
		}
		//min y
		if (y < size.y) {
			size.y = y;
		}
		//max height
		if (y > size.height) {
			size.height = y;
		}
		//min z
		if (z < size.z) {
			size.z = z;
		}
		//max length
		if (z > size.depth) {
			size.depth = z;
		}
	}
	//set center to 0
	size.width -= size.x;
	size.height -= size.y;
	size.depth -= size.z;
	this->size = size;
}
