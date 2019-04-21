#pragma once
//#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H


#include <map>
#include <iostream>
#include <algorithm>

struct Character {
	GLuint	     TextureID;
	glm::ivec2   Size;
	glm::ivec2   Bearing;
	unsigned int Advance;
};

class Font {
public:
	Font();
	Font(const char* file, GLuint size);
	std::map<GLchar, Character> characters;
	glm::ivec2 getVerticalBounds();
private:	
	glm::ivec2 verticalBounds;

	void load(const char* file, GLuint size);
};