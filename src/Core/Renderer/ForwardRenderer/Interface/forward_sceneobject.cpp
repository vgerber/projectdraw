#include "forward_sceneobject.h"

ForwardSceneObject::ForwardSceneObject(SceneObject * sceneObject) : RenderObject(sceneObject) {
    setup();
}

void ForwardSceneObject::draw() {
    for(auto child : children) {
        static_cast<ForwardSceneObject*>(child)->draw();
    }
}

void ForwardSceneObject::setCamera(const Camera &camera) {
    mvp = camera.getProjectionMatrix() * camera.getViewMatrix() * getLinkedObject()->getWorldTransform().getMatrix();
    for(auto child : children) {
        static_cast<ForwardSceneObject*>(child)->setCamera(camera);
    }
}

void ForwardSceneObject::setup() {
}