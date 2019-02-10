#pragma once

//#define GLEW_STATIC
#include <GL/glew.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>


enum TextureFormat {
	R,
	RG,
	RGB,
	RGBA
};

enum TextureDataType {
	UByte,
	Float,
	Int,

};

class Texture {
public:
	Texture();
	Texture(const char * path);
	Texture(void * data, int width, int height, TextureFormat format, TextureDataType dataType);


	virtual GLuint getGLTexture() const;

	int getWidth() const;
	int getHeight() const;
	int getChannelsCount() const;
	TextureFormat getFormat();
	TextureDataType getDataType();

	virtual void activate(int textureOffset = 0);

	virtual void dispose();
protected:
	GLuint texture;
	std::string name;

	int width = 0, height = 0, nrChannels = 0;
};