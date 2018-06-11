#pragma once

#include <string>
#include <vector>
#include "../Resource/loader.h"

enum ShaderType {
	VERTEX,
	FRAGMENT,
	GEOMETRY
};

struct ShaderLayer {
	std::string path;
	ShaderType type;
};

class Shader
{
public:	
	std::vector<ShaderLayer> layers;

	Shader();

	GLuint get_id();
	void load();
	void use();

private:
	GLuint programm_id;
};
