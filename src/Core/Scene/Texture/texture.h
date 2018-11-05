#pragma once

//#define GLEW_STATIC
#include <GL/glew.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Texture {
public:
	Texture();
	Texture(const char * path);

	virtual GLuint getGLTexture();

	int getWidth();
	int getHeight();
	int getChannelsCount();

	virtual void activate(int tex_id);

	void dispose();
protected:
	GLuint m_texture;

	int m_width = 0, m_height = 0, m_nrChannels = 0;
};