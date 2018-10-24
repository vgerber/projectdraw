#include "texture.h"

#define STB_IMAGE_IMPLEMENTION

Texture::Texture(const char * path)
{
	unsigned char * m_imageData = stbi_load(path, &m_width, &m_height, &m_nrChannels, 0);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (m_imageData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("[TEXTURE] [ERROR] failed to load %s\n", path);
	}
	stbi_image_free(m_imageData);
}

GLuint Texture::getGLTexture()
{
	return m_texture;
}

int Texture::getWidth()
{
	return m_width;
}

int Texture::getHeight()
{
	return m_height;
}

int Texture::getChannelsCount()
{
	return m_nrChannels;
}

void Texture::activate(int tex_id)
{
	glActiveTexture(GL_TEXTURE0 + tex_id);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::dispose()
{
	glDeleteTextures(1, &m_texture);
}
