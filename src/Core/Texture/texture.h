#pragma once

//#define GLEW_STATIC
#include <GL/glew.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

enum TextureType {
	Diffuse,
	Specular,
	Alpha,
	Normal
};

class Texture {
public:
	Texture();
	Texture(const char * path);
	Texture(void * data, int width, int height, GLenum format, GLenum internalFormat, GLenum dataType);
	Texture(const unsigned int &glTextureRef, int width, int height, GLenum format, GLenum internalFormat, GLenum dataType);

	virtual GLuint getGLTexture() const;
	int getWidth() const;
	int getHeight() const;
	GLenum getFormat() const;
	GLenum getInternalFormat() const;
	GLenum getDataType() const;

	virtual void activate(int textureOffset = 0);

	virtual void dispose();
protected:
	GLuint texture;
	std::string name;

	GLenum format, internalFormat, dataType;

	int width = 0, height = 0;
};