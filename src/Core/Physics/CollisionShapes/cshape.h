#pragma once

#include "../../Scene/drawable.h"

#include <btBulletCollisionCommon.h>



namespace collision {
    class CollisionShape {
    public:
        CollisionShape(btCollisionShape *shape);
        btCollisionShape* get_shape();
        

        void dispose();
    protected:
        btCollisionShape *shape;
    };

    btCollisionShape* generate_cube(Size size);

    btCollisionShape* generate_sphere(GLfloat radius);
}