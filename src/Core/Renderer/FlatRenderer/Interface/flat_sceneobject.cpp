#include "flat_sceneobject.h"

FlatSceneObject::FlatSceneObject(SceneObject * sceneObject) : RenderObject(sceneObject) {
    setup();
}

void FlatSceneObject::draw() {
    for(auto child : children) {
        static_cast<FlatSceneObject*>(child)->draw();
    }
}

void FlatSceneObject::setCamera(const Camera &camera) {
    mvp = camera.getProjectionMatrix() * camera.getViewMatrix() * getLinkedObject()->getWorldTransform().getMatrix();
    for(auto child : children) {
        static_cast<FlatSceneObject*>(child)->setCamera(camera);
    }
}

void FlatSceneObject::setup() {
}