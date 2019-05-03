#include "forward_drawable.h"

ForwardDrawable::ForwardDrawable(Drawable * drawable) : ForwardSceneObject(static_cast<SceneObject * >(drawable)) {
    setup();
}

void ForwardDrawable::update() {
    ForwardSceneObject::update();
}

void ForwardDrawable::draw() {
    ForwardSceneObject::draw();
}

void ForwardDrawable::drawDepthInfo() {
    ForwardSceneObject::drawDepthInfo();
}

void ForwardDrawable::dispose() {
    ForwardSceneObject::dispose();
}

void ForwardDrawable::setup() {
}