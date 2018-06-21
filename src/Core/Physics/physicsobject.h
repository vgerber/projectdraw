#pragma once

#include <btBulletDynamicsCommon.h>

#include "../Scene/sceneobject.h"
#include "CollisionShapes/cshape.h"

#include <map>

typedef  unsigned int (uint);


class RigidBody {
public:
    RigidBody();
    RigidBody(collision::CollisionShape *shape, glm::vec3 center, glm::vec3 rotion, GLfloat mass);
    btRigidBody* get_body();

private:
    collision::CollisionShape *shape;
    btRigidBody* rigid_body;
};




class PhysicsObject {
public:	
    uint add_rigid_body(RigidBody rigid_body);
    RigidBody get_rigid_body(uint id);
    std::map<uint, class RigidBody> get_rigid_bodys();

private:
    uint id_counter = 0;
    std::map<uint, class RigidBody> rigid_bodys;
};