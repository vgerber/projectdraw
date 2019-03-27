#include "drawable.h"

void Drawable::addTexture(const Texture &texture, TextureType type) {
	if(type == TextureType::Diffuse) {
		diffuseTextures.push_back(&texture);
	}
	if(type == TextureType::Specular) {
		specularTextures.push_back(&texture);
	}
	if(type == TextureType::Alpha) {
		alphaTextures.push_back(&texture);
	}
}

void Drawable::removeTexture(Texture &texture) {
	std::remove_if(diffuseTextures.begin(), diffuseTextures.end(), [&texture](const Texture * tex){ return &texture == tex; });
	std::remove_if(alphaTextures.begin(), alphaTextures.end(), [&texture](const Texture * tex){ return &texture == tex; });
	std::remove_if(specularTextures.begin(), specularTextures.end(), [&texture](const Texture * tex){ return &texture == tex; });
}

const std::vector<const Texture*> Drawable::getDiffuseTextures() const {
	return diffuseTextures;
}

const std::vector<const Texture*> Drawable::getAlphaTextures() const {
	return alphaTextures;
}

const std::vector<const Texture*> Drawable::getSpecularTextures() const {
	return specularTextures;
}


void Drawable::drawBox()
{
	/*
	shader.use();
	glUniform4f(glGetUniformLocation(shader.getId(), "color"), 0.0f, 1.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));

	glBindVertexArray(this->boxVAO);
	glDrawArrays(GL_LINES, 0, 30);
	glBindVertexArray(0);
	*/
}

void Drawable::dispose() {
	for(auto child : children) {
		child->dispose();
		delete child;
	}
	diffuseTextures.clear();
	specularTextures.clear();
	alphaTextures.clear();
}

void Drawable::setCameraMatrices(glm::mat4 cView, glm::mat4 cProj)
{
	for (auto child : children) {
		if(Drawable * drawable = dynamic_cast<Drawable*>(child)) {
			drawable->setCameraMatrices(cView, cProj);
		}
	}
	mvp = cProj * cView * getWorldTransform().getMatrix();
}

glm::mat4 Drawable::getMVPMatrix() {
	return mvp;
}

/*
void Drawable::loadBox()
{
	GLfloat max_center_size = 0.0f;
	max_center_size = size.width;
	if (size.height > max_center_size) {
		max_center_size = size.height;
	}
	if (size.depth > max_center_size) {
		max_center_size = size.depth;
	}
	max_center_size *= 0.1f;
	GLfloat vertices_box[] = {
		size.x,				 size.y,			   size.z,
		size.x + size.width, size.y,			   size.z,
		size.x,				 size.y + size.height, size.z + size.depth,
		size.x + size.width, size.y + size.height, size.z + size.depth,
		size.x,				 size.y + size.height, size.z,
		size.x + size.width, size.y + size.height, size.z,
		size.x,				 size.y,			   size.z + size.depth,
		size.x + size.width, size.y,			   size.z + size.depth,

		size.x + size.width, size.y,			   size.z,
		size.x + size.width, size.y,			   size.z + size.depth,
		size.x + size.width, size.y + size.height, size.z,
		size.x + size.width, size.y + size.height, size.z + size.depth,
		size.x,				 size.y + size.height, size.z,
		size.x,				 size.y + size.height, size.z + size.depth,
		size.x,				 size.y,			   size.z,
		size.x,				 size.y,			   size.z + size.depth,

		size.x + size.width, size.y,			   size.z,
		size.x + size.width, size.y + size.height, size.z,
		size.x + size.width, size.y,			   size.z + size.depth,
		size.x + size.width, size.y + size.height, size.z + size.depth,
		size.x,				 size.y,			   size.z + size.depth,
		size.x,				 size.y + size.height, size.z + size.depth,
		size.x,				 size.y,			   size.z,
		size.x,				 size.y + size.height, size.z,
		
		//center axis
		vcenter.x * size.width - max_center_size, vcenter.y * size.height, vcenter.z * size.depth,
		vcenter.x * size.width + max_center_size, vcenter.y * size.height, vcenter.z * size.depth,

		vcenter.x * size.width, vcenter.y * size.height - max_center_size, vcenter.z * size.depth,
		vcenter.x * size.width, vcenter.y * size.height + max_center_size, vcenter.z * size.depth,

		vcenter.x * size.width, vcenter.y * size.height, vcenter.z * size.depth - max_center_size,
		vcenter.x * size.width, vcenter.y * size.height, vcenter.z * size.depth + max_center_size
	};

	glBindVertexArray(boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_box), &vertices_box, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}
*/