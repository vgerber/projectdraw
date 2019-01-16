#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Texture::Texture()
{
}

Texture::Texture(const char * path)
{
	
	unsigned char * imageData = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
	
}

GLuint Texture::getGLTexture()
{
	return texture;
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}

int Texture::getChannelsCount()
{
	return nrChannels;
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
