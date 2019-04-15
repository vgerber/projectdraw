#include "flat_drawable.h"

FlatDrawable::FlatDrawable(Drawable * drawable) : FlatSceneObject(static_cast<SceneObject * >(drawable)) {
    setup();
}

void FlatDrawable::update() {
    FlatSceneObject::update();
}

void FlatDrawable::draw() {
    FlatSceneObject::draw();
}

void FlatDrawable::dispose() {
    FlatSceneObject::dispose();
}

void FlatDrawable::setup() {
}