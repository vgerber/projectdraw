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