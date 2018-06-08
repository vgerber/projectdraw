#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H


#include <map>
#include <iostream>

struct Character {
	GLuint	   TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint	   Advance;
};

class Font {
public:
	Font();
	Font(const char* file, GLuint size);
	std::map<GLchar, Character> characters;
private:	
	void load(const char* file, GLuint size);
};