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


#ifdef _WIN32
const std::string ROOT_DIR = "C:/Users/Vincent/Documents/Projects/C++/ProjectDraw/src";
#endif

#ifdef linux
const std::string ROOT_DIR = "/home/vincent/Development/Cpp/opengl/projectdraw/src";
#endif

const std::string SHADER_FONT = "font";
const std::string SHADER_BASIC = "basic";
const std::string SHADER_SKYBOX = "skybox";
const std::string SHADER_DEPTH = "depth";
const std::string SHADER_DEPTH_CUBE = "depth_cube";
const std::string SHADER_DEFERRED = "deferred";
const std::string SHADER_GEOMETRY = "geometry";

extern std::map<std::string, class Shader> Shaders;

class Loader
{
public:

	Loader();
	~Loader();

	static std::string GetPath(const GLchar* path);

	static GLuint LoadCubemap(std::vector<std::string> faces);
};
