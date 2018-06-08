#pragma once

#include <string>
#include "../Loader/loader.h"

class Shader
{
public:
	Shader();
	Shader(std::string id);
	~Shader();

	void Load(const char* vertex, const char* fragment);
	void Load(const char* vertex, const char* fragment, const char * geometry);
	std::string GetId();
	GLuint GetProgrammId();
	void Use();

private:
	std::string id;
	GLuint programId;
};
