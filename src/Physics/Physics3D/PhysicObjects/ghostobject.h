#pragma once

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "../Scene/sceneobject.h"
#include "CollisionShapes/cshape.h"

class GhostObject : public SceneObject {
public:
    GhostObject();
    void setShape(collision::CollisionShape &cshape);
	btGhostObject* getPtr();

private:
	collision::CollisionShape *chsape = nullptr;
protected:
	btGhostObject * ghostObject = nullptr;
};