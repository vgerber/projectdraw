#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTION
#include <stb_image.h>

class Texture {
public:
	Texture(const char * path);

	GLuint getGLTexture();

	int getWidth();
	int getHeight();
	int getChannelsCount();

	void activate(int tex_id);

	void dispose();
private:
	GLuint m_texture;

	int m_width = 0, m_height = 0, m_nrChannels = 0;
};