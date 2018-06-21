#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class SceneObject
{
public:
	SceneObject();
	~SceneObject();

	virtual glm::vec3 getPosition();
	std::string getId();

	virtual void setPosition(glm::vec3 position);
	void setId(std::string id);

protected:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	std::string id;
};

