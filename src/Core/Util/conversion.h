#pragma once

#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

glm::vec3 toVec3(btVector3 vec);

btVector3 toBtVec3(glm::vec3 vec);