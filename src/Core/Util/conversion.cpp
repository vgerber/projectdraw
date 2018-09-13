#include "conversion.h"

glm::vec3 toVec3(btVector3 vec)
{
	return glm::vec3(vec.getX(), vec.getY(), vec.getZ());
}

btVector3 toBtVec3(glm::vec3 vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}