#include "mesh.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<sTexture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	callUpdate();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
	this->vertices = vertices;
	this->indices = indices;
	callUpdate();
}

Mesh::Mesh(std::string path)
{
	loadMesh(path);
}

Mesh::Mesh()
{
}

void Mesh::applyMeshOffset(glm::vec3 offset)
{
	for (size_t i = 0; i < vertices.size(); i++) {
		vertices[i].Position += offset;
	}
	callUpdate();
}

void Mesh::applyMeshTransformation(Transform transfrom) {
	for (size_t i = 0; i < vertices.size(); i++) {
		vertices[i].Position = glm::vec3(transfrom.getMatrix() * glm::vec4(vertices[i].Position, 1.0));
	}
	callUpdate();	
}

void Mesh::applyMeshRecenter(glm::vec3 centerPoint)
{	
	Size size = getSize();
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


void Mesh::loadMesh(std::string path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	this->processNode(scene->mRootNode, scene);
	callUpdate();
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
	callUpdate();
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

void Mesh::lineTo(Vertex vertex)
{
	//in opengl lines aren't attached vertex by vertex
	//Two vertices are merged to one line
	//Thats why we need to check if the vertex count is even or odd
	//and therefore the last vertex will be added twice so that the next
	//vertex can be connected via line
	if (indices.size() % 2 == 1)
	{
		indices.push_back(vertices.size());
		vertices.push_back(vertex);
	}
	else
	{
		indices.push_back(vertices.size()-1);
		indices.push_back(vertices.size());
		vertices.push_back(vertex);
	}
	callUpdate();
}

void Mesh::lineTo(glm::vec3 position)
{
	lineTo(Vertex(position, settings.customColor));
}

void Mesh::lineTo(std::vector<Vertex> vertices)
{
	for (Vertex p : vertices)
	{
		lineTo(p);
	}
}

void Mesh::line(Vertex p1, Vertex p2)
{
	//prevent connecting to previous vertices
	//see lineTo
	if (vertices.size() % 2 == 1)
	{
		indices.push_back(vertices.size() - 1);
	}
	indices.push_back(vertices.size());
	vertices.push_back(p1);
	indices.push_back(vertices.size());
	vertices.push_back(p2);
	
	callUpdate();
}

void Mesh::line(glm::vec3 p1, glm::vec3 p2)
{
	line(Vertex{p1, settings.customColor}, Vertex{p2, settings.customColor});
}

void Mesh::triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	triangle(
		Vertex(p1, settings.customColor),
		Vertex(p2, settings.customColor),
		Vertex(p3, settings.customColor)
	);

}

void Mesh::triangle(Vertex p1, Vertex p2, Vertex p3) {
	addVertex(p1);
	addVertex(p2);
	addVertex(p3);
}

void Mesh::rect(glm::vec3 position, int width, int height) {
	rect(
		Vertex(position, settings.customColor),
		width,
		height
	);
}

void Mesh::rect(Vertex position, int width, int height) {
	Vertex p1 = position;
	Vertex p2 = position;
	Vertex p3 = position;
	Vertex p4 = position;

	p2.Position.x += width;
	p3.Position.y += height;
	p4.Position.x += width;
	p4.Position.y += height;

	triangle(p1, p2, p3);
	triangle(p4, p3, p2);
}

void Mesh::addVertex(Vertex vertex)
{
	indices.push_back(vertices.size());
	vertices.push_back(vertex);
	callUpdate();
}

void Mesh::addVertex(glm::vec3 position)
{
	addVertex(Vertex{position, settings.customColor});
}

void Mesh::removeVertex(unsigned int index)
{
	vertices.erase(vertices.begin() + index);
	indices.erase(std::remove_if(indices.begin(), indices.end(), [index](const unsigned int &vIndex) { return index == vIndex; }), indices.end());
	callUpdate();
}

std::vector<Vertex> Mesh::getVertices() const
{
	return vertices;
}

void Mesh::setVertices(std::vector<Vertex> vertices)
{
	this->vertices = vertices;
	callUpdate();
}

void Mesh::setIndices(std::vector<unsigned int> indices)
{
	this->indices = indices;
	callUpdate();
}

std::vector<unsigned int> Mesh::getIndices() const
{
	return indices;
}

void Mesh::clear()
{
	vertices.clear();
	indices.clear();
	callUpdate();
}

void Mesh::reloadSize()
{
	Size size;
	bool initRequired = true;
	if (children.size() >= 0) {
		for (auto child : children) {
			Size childSize = child->getSize();
			childSize.x = child->getPosition().x + childSize.x;
			childSize.y = child->getPosition().y + childSize.y;
			childSize.z = child->getPosition().z + childSize.z;
			childSize.width  = childSize.x + childSize.width;
			childSize.height = childSize.y + childSize.height;
			childSize.depth  = childSize.z + childSize.depth;
			if (initRequired) {
				size.x = childSize.x;
				size.y = childSize.y;
				size.z = childSize.z;
				size.width  = childSize.width;
				size.height = childSize.height;
				size.depth  = childSize.depth;
				initRequired = false;
				continue;
			}
			//min x
			if (size.x > childSize.x) {
				size.x = childSize.x;
			}
			//max width
			if (size.width < childSize.width) {
				size.width = childSize.width;
			}
			//min y
			if (size.y > childSize.y) {
				size.y = childSize.y;
			}
			//max height
			if (size.height < childSize.height) {
				size.height = childSize.height;
			}
			//min z
			if (size.z > childSize.z) {
				size.z = childSize.z;
			}
			//max length
			if (size.depth < childSize.depth) {
				size.depth = childSize.depth;
			}
		}
	}
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

void Mesh::callUpdate() {
	if(!isEditMode())
	{
		reloadSize();
	}
	Drawable::callUpdate();
}
