#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Texture::Texture()
{
}

Texture::Texture(const char * path)
{
	int nrChannels;
	unsigned char * imageData = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (imageData) {
		try {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		catch (...) {
			stbi_image_free(imageData);
			printf("[Core][TEXTURE] [ERROR] data fail %s\n", path);
		}
	}
	else {
		printf("[TEXTURE] [ERROR] failed to load %s\n", path);
	}
	stbi_image_free(imageData);
	
	this->internalFormat = GL_RGBA;
	this->format = GL_RGBA;
	this->dataType = GL_UNSIGNED_BYTE;
}


Texture::Texture(void * data, int width, int height, GLenum format, GLenum internalFormat, GLenum dataType)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, data);

	this->width = width;
	this->height = height;
	this->format = format;
	this->internalFormat = internalFormat;
	this->dataType = dataType;
}

Texture::Texture(const unsigned int &glTextureRef, int width, int height, GLenum format, GLenum internalFormat, GLenum dataType) {
	this->texture = glTextureRef;
	this->width = width;
	this->height = height;
	this->dataType = dataType;
	this->format = format;
	this->internalFormat = internalFormat;
}

GLuint Texture::getGLTexture() const
{
	return texture;
}

int Texture::getWidth() const
{
	return width;
}

int Texture::getHeight() const
{
	return height;
}

GLenum Texture::getFormat() const {
	return format;
}

GLenum Texture::getInternalFormat() const {
	return internalFormat;
}

GLenum Texture::getDataType() const {
	return dataType;
}

void Texture::activate(int textureOffset)
{
	glActiveTexture(GL_TEXTURE0 + textureOffset);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::dispose()
{
	glDeleteTextures(1, &texture);
}
