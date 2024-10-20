#include "resourcemanager.h"

std::map<std::string, Shader> ResourceManager::shaders;

std::string ResourceManager::GetPath(std::string path) {
  return Configurator::get(Configurator::EngineSrc) + path;
}

std::string ResourceManager::GetProjectPath(std::string path) {
  return Configurator::get(Configurator::ProjectRoot) + path;
}

void ResourceManager::storeShader(std::string name, Shader shader) {
  shaders[name] = shader;
}

Shader ResourceManager::loadShader(std::string name) { return shaders[name]; }

void ResourceManager::deleteShader(std::string name) {
  auto shader = shaders.find(name);
  if (shader != shaders.end())
    shaders.erase(shader);
}

void ResourceManager::dispose() {}
