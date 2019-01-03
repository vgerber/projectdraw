#include "vehicle.h"

DemoVehicle::DemoVehicle(DiscreteWorld &world, Scene &scene) {
#ifdef _WIN32
    vehicleMesh = Mesh("C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/examples/assets/basic_car.fbx");
#elif linux
	vehicleMesh = Mesh("/home/vincent/Development/Cpp/projectdraw_slim/examples/assets/basic_car.fbx");
#endif
    chassis = static_cast<Mesh*>(vehicleMesh.getChild("Car"));
    chassis->applyMeshRecenter(glm::vec3(0.0f, 0.0f, chassis->getSize().depth * 0.5f));
    scene.addObject(*chassis);
    std::string wheelNames[4] = {"WheelBackL", "WheelBackR", "WheelFrontL", "WheelFrontR"};
    for(int i = 0; i < 4; i++)  {
        wheels[i] = static_cast<Mesh*>(vehicleMesh.getChild(wheelNames[i]));
        scene.addObject(*wheels[i]);
    }
    
    btCompoundShape * chassisShape = new btCompoundShape();
    btTransform transform = btTransform::getIdentity();
    transform.setOrigin(btVector3(0.0f, 0.0f, chassis->getSize().depth * 0.5));
    btCollisionShape * chassisShapePart = collision::generateCube(chassis->getSize());
    chassisShape->addChildShape(transform, chassisShapePart);
    //chassisShape->addChildShape(transform, collision::generateCube(chassis->getSize()));
    shape = new collision::CollisionShape(chassisShape);
    vehicle = new RaycastVehicle(*shape, 1000.0, world);
    vehicle->setMaxForwardForce(1500.0f);
    vehicle->setMaxSteeringAngle(glm::radians(80.0f));
    vehicle->setMaxBrakeForce(40.0);
    world.addCollisionObject(*vehicle);

    vehicle->linkDrawable(*chassis);
    //vehicle->setPosition(0.0, 0.0, 4.0);
    glm::vec3 contactPoints[4] = { 
        glm::vec3( 1.9,  0.8, -0.1),
        glm::vec3( 1.9, -0.8, -0.1),
        glm::vec3(-1.9,  0.8, -0.1),
        glm::vec3(-1.9, -0.8, -0.1)        
    };
    for(int i = 0; i < 4; i++) {
        RaycastVehicleWheel wheel;
        wheel.canSteer = i < 2;
        wheel.canAccelerate = true;
        wheel.canBrake = i >= 2;
        wheel.contactPoint = contactPoints[i];
        wheel.radius = wheels[i]->getSize().depth * 0.5;
        wheel.width = wheels[i]->getSize().width;
        wheel.suspensionStiffness = 20.0;
        wheel.suspensionRestLength = 0.1;
        wheel.dampingRelaxation = 1.0;
        wheel.dampingCompression = 1.0;
        wheel.drawable = wheels[i];
        vehicle->addWheel(wheel);
    }
}

RaycastVehicle * DemoVehicle::getVehicleHandle() {
    return vehicle;
}

void DemoVehicle::update(float delta) {
    printf("x=%f\n", vehicle->getWorldTransform().getTranslation().x);
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
        vehicle->steer(-0.8);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        vehicle->steer(0.8);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        vehicle->brake(1.0);
    }
}