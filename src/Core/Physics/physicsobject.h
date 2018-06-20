#pragma once

#include <btBulletDynamicsCommon.h>


#include <map>


struct RigidBody {
    btRigidBody* rigid_body;
};

class PhysicsObject {
public:
    uint add_rigid_body(RigidBody rigid_body);
    RigidBody get_rigid_body(unsigned int id);
    std::map<uint, RigidBody> get_rigid_bodys();

private:
    uint id_counter = 0;
    std::map<uint, RigidBody> rigid_bodys;
};