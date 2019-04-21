#include "deferred_drawable.h"

DeferredDrawable::DeferredDrawable(Drawable * drawable) : DeferredSceneObject(static_cast<SceneObject * >(drawable)) {
    setup();
}

void DeferredDrawable::update() {
    DeferredSceneObject::update();
}

void DeferredDrawable::draw() {
    DeferredSceneObject::draw();
}

void DeferredDrawable::drawRaw(Shader shader) {
    DeferredSceneObject::drawRaw(shader);
}

void DeferredDrawable::drawMesh(Shader shader) {
    DeferredSceneObject::drawMesh(shader);
}

void DeferredDrawable::dispose() {
    DeferredSceneObject::dispose();
}

void DeferredDrawable::setup() {
}