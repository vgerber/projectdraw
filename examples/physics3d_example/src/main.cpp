#include "Core/common.h"
#include "Physics/Physics3D/World/discreteworld.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

const int WIDTH = 1000;
const int HEIGHT = 1000;

sf::Window * activeWindow = nullptr;
sf::Clock windowClock;
float deltaTime = 0.0f;

//returns false if activeWindow is closed
bool handleEvent();

int main() {

    sf::ContextSettings ctxSetting;
    ctxSetting.majorVersion = 3;
    ctxSetting.minorVersion = 3;
    ctxSetting.depthBits = 24;
    ctxSetting.stencilBits = 8;

    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Physics3D", sf::Style::Default, ctxSetting);
    activeWindow = &window;
    initCore();

    Scene mainScene(WIDTH, HEIGHT);


    PerspectiveCamera freeCamera;
    freeCamera.FarZ = 30.0f;
    freeCamera.setPosition(glm::vec3(20.0f, 0.0f, 7.0f));
    freeCamera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    mainScene.addObject(freeCamera);

    Drawable ground;
    ground.setModel(primitives::generateQuad(10.0f, 10.0f, 0.2f, glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)));
    ground.setPosition(glm::vec3(0.0f, 0.0f, -0.1f));
    mainScene.addObject(ground);

    Drawable testObject;
    testObject.setModel(primitives::generateSphere(20, 20,  glm::vec4(0.4f, 0.8f, 0.4f, 1.0f)));
    testObject.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    mainScene.addObject(testObject);

    DirectionalLight sunLight;
    sunLight.change_direction(glm::vec3(-0.3, -1.0f, -1.0));
    sunLight.diffuse = glm::vec3(1.0f, 1.0f, 0.8f);
    sunLight.ambient = sunLight.diffuse * 0.5f;
    sunLight.shadow = true;
    mainScene.addObject(sunLight);
    
    
    
    DiscreteWorld physicsWorld;

    collision::CollisionShape rTestBodyShape(collision::generateSphere(0.5f));
    RigidBody rTestBody(rTestBodyShape, 1.0f, RigidType::DYNAMIC);
    rTestBody.linkDrawable(testObject);
    rTestBody.setRestitution(1.0);
    physicsWorld.addPhysicsObject(rTestBody);



    collision::CollisionShape rGroundShape(collision::generateCube(ground.getSize()));
    RigidBody rGroundBody(rGroundShape, 0.0f, RigidType::KINEMAITC);
    rGroundBody.linkDrawable(ground);
    rGroundBody.setRestitution(0.8);
    physicsWorld.addPhysicsObject(rGroundBody);


    sf::Time elapsedTime = windowClock.getElapsedTime();
    deltaTime = 0.0f;
    while(window.isOpen()) {
        if(!handleEvent()) {
            break;
        }
        clearScreen(glm::vec4(0.3f, 0.3f, 1.0f, 1.0f));

		float deltaTimeMilli = deltaTime * 0.001f;

		//Keyboard events
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			ground.setPosition(ground.getPosition() + glm::vec3(0.0f, 0.0f, -10.0f * deltaTimeMilli));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			ground.setPosition(ground.getPosition() + glm::vec3(0.0f, 0.0f, 10.0f * deltaTimeMilli));
		}
		rGroundBody.refreshBody();


        //update all elements
        physicsWorld.update(std::max(deltaTime * 0.001f, 0.002f));
        mainScene.update(deltaTime);
        activeWindow->display();

        //recaluclate deltaTime
        deltaTime = (windowClock.getElapsedTime()- elapsedTime).asMilliseconds();
        elapsedTime = windowClock.getElapsedTime();
    }

    return 0;
}


bool handleEvent() {
    sf::Event e;
    while(activeWindow->pollEvent(e)) {
        if(e.type == sf::Event::Closed) {
            activeWindow->close();
            return false;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            activeWindow->close();
            return false;
        }
    }

    return true;
}