#pragma once

#include "Physics/Physics3D/CollisionObject/Vehicle/raycastvehicle.h"
#include "Core/Scene/Scenes/scene.h"

#include <SFML/Window.hpp>

class DemoVehicle {
public:
    DemoVehicle(DiscreteWorld &world, Scene &scene);

    void update(float delta);

private:
    Drawable chassis;
    Drawable wheels[4];
    collision::CollisionShape * shape;
    RaycastVehicle * vehicle;
};