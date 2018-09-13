#pragma once
#define GLEW_STATIC
#include <GL/glew.h>

#include <SFML/Graphics.hpp>

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
const std::string SHADER_TEXTURE = "texture";
const std::string SHADER_BASIC = "basic";
const std::string SHADER_SKYBOX = "skybox";
const std::string SHADER_DEPTH = "depth";
const std::string SHADER_DEPTH_CUBE = "depth_cube";
const std::string SHADER_DEPTH_PERSP = "depth_persp";
const std::string SHADER_DEFERRED = "deferred";
const std::string SHADER_GEOMETRY = "geometry";
const std::string SHADER_DEFFERED_LIGHT = "ligth";
const std::string SHADER_DEFFERED_PLIGHT_NOS = "deferred_pligth_nos";
const std::string SHADER_DEFFERED_SLIGHT_NOS = "deferred_sligth_nos";
const std::string SHADER_DEFFERED_NORMALS = "normals";
const std::string SHADER_DEFFERED_GEOMETRY = "deferred_geometry";
const std::string SHADER_FILTER_BLUR = "filter_blur";
const std::string SHADER_INSTANCING_BASIC = "instancing_basic";

extern std::map<std::string, class Shader> Shaders;

class Loader
{
public:

	Loader();
	~Loader();

	static std::string GetPath(const GLchar* path);

	static GLuint LoadCubemap(std::vector<std::string> faces);
};
