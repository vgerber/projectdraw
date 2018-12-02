#include "ghostobject.h"

GhostObject::GhostObject() {

}

void GhostObject::setShape(collision::CollisionShape &cshape) {
    if(!ghostObject) {
        ghostObject = new btGhostObject();
    }
    ghostObject->setCollisionShape(cshape.getShape());
	this->chsape = &cshape;
}

btGhostObject * GhostObject::getPtr()
{
	return ghostObject;
}

