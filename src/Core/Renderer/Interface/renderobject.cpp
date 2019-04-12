#include "renderobject.h"

RenderObject::RenderObject(SceneObject * sceneObject) {
    this->sceneObject = sceneObject;
    sceneObject->addUpdateListener(this, std::bind(&RenderObject::update, this));
    setup();
}

void RenderObject::update() {
        
}

void RenderObject::dispose() {
    for(auto child : children) {
        child->dispose();
        child->getLinkedObject()->removeUpdateListener(this);
        delete child;
    }
    children.clear();
}

SceneObject * RenderObject::getLinkedObject() const {
    return sceneObject;
}

bool RenderObject::operator==(const RenderObject &a) const {
    return sceneObject == a.sceneObject;
}

void RenderObject::setup() {
    printf("%s\n", sceneObject->getId().c_str());
}