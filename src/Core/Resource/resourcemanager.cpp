#include "resourcemanager.h"


std::map<std::string, Shader> ResourceManager::shaders;


std::string ResourceManager::GetPath(const GLchar * path)
{
	std::string complete_path = ROOT_DIR;
	complete_path += path;
	return complete_path;
}

void ResourceManager::storeShader(std::string name, Shader shader)
{
	shaders[name] = shader;
}

Shader ResourceManager::loadShader(std::string name)
{
	return shaders[name];
}

void ResourceManager::deleteShader(std::string name)
{
	auto shader = shaders.find(name);
	if (shader != shaders.end())
		shaders.erase(shader);
}

void ResourceManager::free()
{
}
