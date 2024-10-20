#include "conversion.h"

btVector3 toBtVec3(glm::vec3 vec) { return btVector3(vec.x, vec.y, vec.z); }