#pragma once

#include <map>

#include "../Physics/physics.h"
#include "../Shader/shader.h"

class ResourceManager {
public:

	static void storeRigidBody(RigidBody &rigidBody);
	static RigidBody * loadRigidBody(btRigidBody *rigidBody);
	static void deleteRigidBody(RigidBody &rigidBody);

	static void storeShader(std::string name, Shader shader);
	static Shader loadShader(std::string name);
	static void deleteShader(std::string name);

	static void free();
private:
	static std::vector<RigidBody*> rigidBodys;
	static std::map<std::string, Shader> shaders;
};