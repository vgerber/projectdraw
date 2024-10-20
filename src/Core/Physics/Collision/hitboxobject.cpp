#include "hitboxobject.h"

bool HitboxObject::isHit()
{
	return ghostObject->getNumOverlappingObjects() > 0;
}

std::vector<RigidBody> HitboxObject::getOverlapping()
{
	return std::vector<RigidBody>();
}
