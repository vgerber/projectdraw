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