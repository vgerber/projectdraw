#pragma once

#include <map>

#include "../Shader/shader.h"

class ResourceManager {
public:
	static void storeShader(std::string name, Shader shader);
	static Shader loadShader(std::string name);
	static void deleteShader(std::string name);

	static void free();
private:
	static std::map<std::string, Shader> shaders;
};