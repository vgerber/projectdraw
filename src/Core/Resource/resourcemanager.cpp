#include "resourcemanager.h"


std::vector<RigidBody*> ResourceManager::rigidBodys;
std::map<std::string, Shader> ResourceManager::shaders;

void ResourceManager::storeRigidBody(RigidBody & rigidBody)
{
	rigidBodys.push_back(&rigidBody);
}

RigidBody * ResourceManager::loadRigidBody(btRigidBody * rigidBody)
{
	for (size_t i = 0; i < rigidBodys.size(); i++) {
		RigidBody* rbody = rigidBodys[i];
		if (rigidBody == rbody->getBody()) {
			return rbody;
		}
	}
	return nullptr;
}

void ResourceManager::deleteRigidBody(RigidBody & rigidBody)
{
	for (size_t i = 0; i < rigidBodys.size(); i++) {
		RigidBody* rbody = rigidBodys[i];
		if (rigidBody.getBody() == rbody->getBody()) {
			rigidBodys.erase(rigidBodys.begin() + i);
			i--;
		}
	}
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
