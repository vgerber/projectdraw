#pragma once

#include "Physics/Physics3D/CollisionObject/Vehicle/raycastvehicle.h"
#include "Core/Scene/Scenes/scene.h"

#include <SFML/Window.hpp>

class DemoVehicle {
public:
    DemoVehicle(DiscreteWorld &world, Scene &scene);

    RaycastVehicle * getVehicleHandle();

    void update(float delta);

private:
    Mesh vehicleMesh;
    Mesh * chassis;
    Mesh * wheels[4];
    collision::CollisionShape * shape;
    RaycastVehicle * vehicle;
};