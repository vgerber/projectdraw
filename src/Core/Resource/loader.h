#pragma once
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "SOIL.h"

#include "../Shader/shader.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <map>

class Loader
{
public:
	static std::map<std::string, Shader> Shaders;

	Loader();
	~Loader();

	static std::string GetPath(const GLchar* path);

	static GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

	static GLuint LoadGeometryShader(const char * vertex_file_path, const char * fragment_file_path, const char * geometry_file_path);

	static GLuint LoadCubemap(std::vector<std::string> faces);

	static const std::string ROOT_DIR;
};