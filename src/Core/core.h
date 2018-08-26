#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>

#include <btBulletDynamicsCommon.h>

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "Resource/loader.h"
#include "Shader/shader.h"


void init_core();

glm::vec3 toVec3(btVector3 vec);

btVector3 toBtVec3(glm::vec3 vec);

