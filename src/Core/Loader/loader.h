#pragma once
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "SOIL.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class Loader
{
public:
	Loader();
	~Loader();

	static std::string GetPath(const GLchar* path);

	static GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

	static GLuint LoadGeometryShader(const char * vertex_file_path, const char * fragment_file_path, const char * geometry_file_path);
	
	static GLuint LoadCubemap(std::vector<std::string> faces);

	static const std::string ROOT_DIR;
};

