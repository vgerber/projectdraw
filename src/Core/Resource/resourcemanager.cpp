#include "resourcemanager.h"


std::map<std::string, Shader> ResourceManager::shaders;


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
