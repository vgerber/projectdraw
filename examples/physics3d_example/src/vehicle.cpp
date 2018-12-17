#include "vehicle.h"

DemoVehicle::DemoVehicle(DiscreteWorld &world, Scene &scene) {
    chassis.setModel(primitives::generateQuad(2.0, 3.0, 1.0, glm::vec4(1.0, 0.5, 0.5, 1.0)));
    scene.addObject(chassis);
    for(int i = 0; i < 4; i++)  {
        wheels[i].setModel(primitives::generateQuad(0.2, 1.0, 1.0, glm::vec4(0.2, 0.2, 0.2, 1.0)));
        scene.addObject(wheels[i]);
    }
    
    shape = new collision::CollisionShape(collision::generateCube(chassis.getSize()));
    vehicle = new RaycastVehicle(*shape, 1000.0, world);
    vehicle->setMaxForwardForce(3000.0f);
    vehicle->setMaxSteeringAngle(glm::radians(80.0f));
    vehicle->setMaxBrakeForce(40.0);
    world.addCollisionObject(*vehicle);

    vehicle->linkDrawable(chassis);
    glm::vec3 contactPoints[4] = { 
        glm::vec3(1.0, 1.0, -0.4),
        glm::vec3(-1.0, 1.0, -0.4),
        glm::vec3(1.0, -1.0, -0.4),
        glm::vec3(-1.0, -1.0, -0.4)        
    };
    for(int i = 0; i < 4; i++) {
        RaycastVehicleWheel wheel;
        wheel.canSteer = i < 2;
        wheel.canAccelerate = true;
        wheel.canBrake = i >= 2;
        wheel.contactPoint = contactPoints[i];
        wheel.radius = wheels[i].getSize().depth * 0.5;
        wheel.width = wheels[i].getSize().width;
        wheel.suspensionStiffness = 10.0;
        wheel.suspensionRestLength = 0.05;
        wheel.dampingRelaxation = 1.0;
        wheel.dampingCompression = 1.0;
        wheel.drawable = &wheels[i];
        vehicle->addWheel(wheel);
    }
}

void DemoVehicle::update(float delta) {
    vehicle->steer(0.0);
    vehicle->move(0.0);
    vehicle->brake(0.0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        vehicle->move(1.0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        vehicle->move(-1.0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        vehicle->steer(0.5);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        vehicle->steer(-0.5);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        vehicle->brake(1.0);
    }
}