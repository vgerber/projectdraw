#pragma once

#include "../ghostobject.h"
#include "../rigidbody.h"
#include "../../Resource/resourcemanager.h"

class HitboxObject : public GhostObject {
public:
	bool isHit();
	std::vector<RigidBody> getOverlapping();
};