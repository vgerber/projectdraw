#include "deferred_sceneobject.h"

DeferredSceneObject::DeferredSceneObject(SceneObject * sceneObject) : RenderObject(sceneObject) {
    setup();
}

void DeferredSceneObject::draw() {
    for(auto child : children) {
        static_cast<DeferredSceneObject*>(child)->draw();
    }
}

void DeferredSceneObject::drawRaw(Shader shader) {
    for(auto child : children) {
        static_cast<DeferredSceneObject*>(child)->drawRaw(shader);
    }
}

void DeferredSceneObject::drawMesh(Shader shader) {
    for(auto child : children) {
        static_cast<DeferredSceneObject*>(child)->drawMesh(shader);
    }
}

void DeferredSceneObject::setCamera(const Camera &camera) {
    mvp = camera.getProjectionMatrix() * camera.getViewMatrix() * getLinkedObject()->getWorldTransform().getMatrix();
    for(auto child : children) {
        static_cast<DeferredSceneObject*>(child)->setCamera(camera);
    }
}

void DeferredSceneObject::setup() {
}