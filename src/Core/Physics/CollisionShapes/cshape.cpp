#include "cshape.h"

using namespace collision;

CollisionShape::CollisionShape(btCollisionShape* shape) {
    this->shape = shape;
}


btCollisionShape* CollisionShape::get_shape() {
    return shape;
}

void CollisionShape::dispose() {
    delete shape;
}

btCollisionShape* collision::generate_cube(Size size) {
    return new btBoxShape(btVector3(size.width * .5f, size.height * .5f, size.depth * .5f));
}

btCollisionShape* collision::generate_sphere(GLfloat radius) {
    return new btSphereShape(btScalar(radius));
}