#include "ghostobject.h"

GhostObject::GhostObject() {

}

void GhostObject::setShape(btCollisionShape* shape) {
    if(!ghostObject) {
        ghostObject = new btGhostObject();
    }
    ghostObject->setCollisionShape(shape);
}