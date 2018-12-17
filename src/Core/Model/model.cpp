#include "model.h"

GLint TextureFromFile(const char* path, std::string directory);

Model::Model()
{
}

Model::Model(GLchar * path)
{
	loadModel(path);
}

Model::Model(std::vector<BasicMesh> meshes, std::vector<sTexture> textures)
{
	this->meshes = meshes;
	this->textures_loaded = textures;
}

Model::~Model()
{
}

void Model::setMesh(int meshIndex, BasicMesh mesh)
{
	meshes[meshIndex] = mesh;
	meshes[meshIndex].resize();
}

Size Model::getSize()
{
	Size size;
	GLfloat x, y, z;
	bool is_first = true;
	for (BasicMesh mesh : meshes) {
		for (Vertex vertex : mesh.getVertices()) {
			x = vertex.Position.x;
			y = vertex.Position.y;
			z = vertex.Position.z;
			if (is_first) {
				size.x = x;
				size.y = y;
				size.z = z;
				size.width = 0.0;
				size.height = 0.0;
				size.depth = 0.0;
				is_first = false;
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
	}
	//set center to 0
	size.width  -= size.x;
	size.height -= size.y;
	size.depth  -= size.z;
	size.x = -0.5f * size.width  - size.x;
	size.y = -0.5f * size.height - size.y;
	size.z = -0.5f * size.depth  - size.z;
	glm::vec3 offset(size.x, size.y, size.z);
	for (size_t i = 0; i < meshes.size(); i++) {
		meshes[i].add_offset(offset);
	}
	size.x = 0.0f;
	size.y = 0.0f;
	size.z = 0.0f;
	
	return size;
}

std::vector<BasicMesh> Model::getMeshes()
{
	return meshes;
}

void Model::draw(Shader shader, DrawType drawType)
{
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		this->meshes[i].draw(shader, drawType);
	}
}

void Model::drawInstancing(Shader shader, DrawType drawType, int amount)
{
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		this->meshes[i].drawInstancing(shader, drawType, amount);
	}
}

void Model::drawNormals(Shader shader)
{
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		this->meshes[i].drawNormals(shader);
	}
}

void Model::dispose() {
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		this->meshes[i].dispose();
	}
	this->meshes.clear();
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	this->directory = path.substr(0, path.find_last_of('/'));
	
	this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	{
		printf("\nHead Node %s %d\n", node->mName.C_Str(), node->mNumChildren);
		aiMatrix4x4 transform = node->mTransformation;
		printf("[%f %f %f %f]\n", transform.a1, transform.a2, transform.a3, transform.a4);
		printf("[%f %f %f %f]\n", transform.b1, transform.b2, transform.b3, transform.b4);
		printf("[%f %f %f %f]\n", transform.c1, transform.c2, transform.c3, transform.c4);
		printf("[%f %f %f %f]\n\n", transform.d1, transform.d2, transform.d3, transform.d4);
	}
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		/*aiMatrix4x4 transform = node->mTransformation;
		printf("Mesh Node %s\n", node->mName.C_Str());
		printf("Transfrom \n");
		printf("[%f %f %f %f]\n", transform.a1, transform.a2, transform.a3, transform.a4);
		printf("[%f %f %f %f]\n", transform.b1, transform.b2, transform.b3, transform.b4);
		printf("[%f %f %f %f]\n", transform.c1, transform.c2, transform.c3, transform.c4);
		printf("[%f %f %f %f]\n", transform.d1, transform.d2, transform.d3, transform.d4);*/
		this->meshes.push_back(this->process_mesh(mesh, scene));
	}
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		Model * childModel = new Model();
		childModel->processNode(node->mChildren[i], scene);
		subModels.push_back(childModel);
	}
}

BasicMesh Model::process_mesh(aiMesh * mesh, const aiScene * scene)
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
	if (mesh->mMaterialIndex >= 0) {
		material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<sTexture> diffuseMaps = this->load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.begin(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<sTexture> specularMaps = this->load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.begin(), specularMaps.begin(), specularMaps.end());
	}
	return BasicMesh(vertices, indices, textures);
}

std::vector<sTexture> Model::load_material_textures(aiMaterial * mat, aiTextureType type, std::string typeName)
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

GLint TextureFromFile(const char* path, std::string directory)
{
	/*
	//Generate texture ID and load texture data 
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
	*/
	return -1;
}
