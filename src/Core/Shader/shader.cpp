#include "shader.h"


Shader::Shader()
{
	id = "undef";
}

Shader::Shader(std::string id)
{
	this->id = id;
}

Shader::~Shader()
{
}

void Shader::Load(const char * vertex, const char * fragment)
{
	programId = Loader::LoadShaders(vertex, fragment);
}

void Shader::Load(const char * vertex, const char * fragment, const char * geometry)
{
	programId = Loader::LoadGeometryShader(vertex, fragment, geometry);
}

std::string Shader::GetId()
{
	return id;
}

GLuint Shader::GetProgrammId()
{
	return programId;
}

void Shader::Use()
{
	glUseProgram(programId);
}
