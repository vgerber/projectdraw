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

const std::string SHADER_FONT = "font";
const std::string SHADER_BASIC = "basic";
const std::string SHADER_SKYBOX = "skybox";
const std::string SHADER_DEPTH = "depth";
const std::string SHADER_DEPTH_CUBE = "depth_cube";
const std::string SHADER_DEFERRED = "deferred";
const std::string SHADER_GEOMETRY = "geometry";

class Loader
{
public:
	static std::map<std::string, Shader> Shaders;

	Loader();
	~Loader();

	static std::string GetPath(const GLchar* path);

	static GLuint LoadCubemap(std::vector<std::string> faces);

	static const std::string ROOT_DIR;
};