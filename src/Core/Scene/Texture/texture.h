#pragma once

//#define GLEW_STATIC
#include <GL/glew.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

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
	GLuint texture;
	std::string name;

	int width = 0, height = 0, nrChannels = 0;
};