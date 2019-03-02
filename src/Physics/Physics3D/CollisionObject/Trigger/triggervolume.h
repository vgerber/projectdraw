#pragma once
#include <vector>
#include <functional>

#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include "Core/Util/Transform/moveable.h"
#include "Physics/Physics3D/CollisionObject/collisionobject.h"
#include "Physics/Physics3D/Collision/Shapes/cshape.h"
#include "Physics/Physics3D/World/collisionworld.h"

class TriggerVolume : public CollisionObject {
public:

    TriggerVolume(collision::CollisionShape shape, glm::vec3 position);

    virtual void update() override;

    std::vector<CollisionResult> getCollisions();

    btCollisionObject * getCollisionObjectHandle() override;
private:
    int lastOverlappingObjects = 0;
    btGhostObject * ghostObject;

    virtual void transformChanged() override;

};
