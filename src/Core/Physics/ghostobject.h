#pragma once

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "../Scene/sceneobject.h"

class GhostObject : public SceneObject {
public:
    GhostObject();
    void setShape(btCollisionShape* shape);
private:
    btGhostObject* ghostObject = nullptr;
};