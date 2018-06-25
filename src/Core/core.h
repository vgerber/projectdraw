#pragma once
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include "Resource/loader.h"
#include "Shader/shader.h"


void init_core();

glm::vec3 toVec3(btVector3 vec);

btVector3 toBtVec3(glm::vec3 vec);

