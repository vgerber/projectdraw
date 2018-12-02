#pragma once

#include <btBulletCollisionCommon.h>
#include <glm/glm.hpp>


glm::vec3 toGLMVec3(btVector3 vec);

btVector3 toBtVec3(glm::vec3 vec);