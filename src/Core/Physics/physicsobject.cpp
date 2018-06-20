#include "physicsobject.h"

uint PhysicsObject::add_rigid_body(RigidBody rigid_body) {
    rigid_bodys[id_counter] = rigid_body;
    return id_counter++;
}

RigidBody PhysicsObject::get_rigid_body(uint id) {
    return rigid_bodys[id];
}

std::map<uint, RigidBody> PhysicsObject::get_rigid_bodys() {
    return rigid_bodys;
}