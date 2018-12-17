#include "Core/common.h"
#include "Physics/Physics3D/World/discreteworld.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "vehicle.h"

const int WIDTH  = 500;
const int HEIGHT = 500;

sf::Window * activeWindow = nullptr;
sf::Clock windowClock;
float deltaTime = 0.0f;

//returns false if activeWindow is closed
bool handleEvent(Camera &camera, Scene &scene);

std::vector<float> getHeightField(int width,  int length);

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
    freeCamera.FarZ = 120.0f;
    freeCamera.setPosition(glm::vec3(40.0f, 0.0f, 15.0f));
    freeCamera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    mainScene.addObject(freeCamera);

    Drawable ground;
    ground.setModel(primitives::generateQuad(40.0f, 40.0f, 0.2f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)));
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
    mainScene.addObject(*physicsWorld.getDebugDrawable());

    collision::CollisionShape rTestBodyShape(collision::generateSphere(0.5f));
    RigidBody rTestBody(rTestBodyShape, 1.0f, RigidType::DYNAMIC);
    rTestBody.linkDrawable(testObject);
    rTestBody.setRestitution(1.0);
    physicsWorld.addCollisionObject(rTestBody);



    collision::CollisionShape rGroundShape(collision::generateCube(ground.getSize()));
    RigidBody rGroundBody(rGroundShape, 0.0f, RigidType::KINEMAITC);
    rGroundBody.linkDrawable(ground);
    rGroundBody.setRestitution(0.5);
    rGroundBody.onCollide = [&rTestBody](const CollisionObject* collisionObject) {
        if(collisionObject == &rTestBody) {
            //printf("TestBody Bounce!\n");
            rTestBody.applyImpulse(glm::vec3(0.0, 0.0, 0.2));
        }
    };
    physicsWorld.addCollisionObject(rGroundBody);

    collision::CollisionShape tBoxShape(collision::generateCube(Size{0.0, 0.0, 0.0, 3.0, 3.0, 1.0}));
    TriggerVolume tTestTrigger(tBoxShape, glm::vec3(0.0, 0.0, 2.0));
    tTestTrigger.onCollide = [](const CollisionObject *) {
        //printf("Object(s) entered!\n");
    };
    physicsWorld.addCollisionObject(tTestTrigger);

    DemoVehicle vehicle1(physicsWorld, mainScene);

    int width = 100;
    int height = 100;
    std::vector<float> heightData = getHeightField(width, height);

    Drawable terrain;
    terrain.setModel(primitives::generateHeightfield(width, height, heightData));
    mainScene.addObject(terrain);

    collision::CollisionShape rTerrainShape(collision::generateHeightField(heightData, width, height));
    RigidBody rTerrain(rTerrainShape, 0.0, RigidType::STATIC);
    rTerrain.linkDrawable(terrain);
    physicsWorld.addCollisionObject(rTerrain);



    sf::Time elapsedTime = windowClock.getElapsedTime();
    deltaTime = 0.0f;
    while(window.isOpen()) {
        if(!handleEvent(freeCamera, mainScene)) {
            break;
        }
        clearScreen(glm::vec4(0.3f, 0.3f, 1.0f, 1.0f));

		float deltaTimeMilli = deltaTime * 0.001f;

		//Keyboard events
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			ground.setPosition(ground.getPosition() + glm::vec3(0.0f, 0.0f, -5.0f * deltaTimeMilli));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			ground.setPosition(ground.getPosition() + glm::vec3(0.0f, 0.0f, 5.0f * deltaTimeMilli));
		}
		rGroundBody.refreshBody();

        RaycastHitResult hitResult = physicsWorld.rayCastAll(glm::vec3(0.0, 10.0, 2.0), glm::vec3(0.0, -10.0, 2.0));
        //printf("Raycast Hits %d\n", hitResult.hitLocations.size());


        //update all elements
        vehicle1.update(deltaTimeMilli);
        physicsWorld.update(std::max(deltaTime * 0.001f, 0.001f));
        //static_cast<Geometry*>(physicsWorld.getDebugDrawable())->line(hitResult.castStartLocation, hitResult.castEndLocation);
        mainScene.update(deltaTime);


        activeWindow->display();

        //recaluclate deltaTime
        deltaTime = (windowClock.getElapsedTime()- elapsedTime).asMilliseconds();
        elapsedTime = windowClock.getElapsedTime();
    }

    return 0;
}


bool handleEvent(Camera &camera, Scene &scene) {
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
		if (e.type == sf::Event::Resized) {
			sf::Vector2u size = activeWindow->getSize();
			scene.resize(size.x, size.y);
			camera.Width = size.x;
			camera.Height = size.y;
		}
    }

    return true;
}

std::vector<float> getHeightField(int width, int height) {
    std::vector<float> data;

    float halfX = width / 2;
    float halfY = height / 2;
    float offset = 30.0;
    float maxValue = 20.0;

    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            float xVal = (abs(x - halfX)) - offset;
            xVal = std::max(xVal, 0.0f);
            xVal = maxValue * (xVal / (halfX - offset));
            float yVal = (abs(y - halfY)) - offset;
            yVal = std::max(yVal, 0.0f);
            yVal = maxValue * (yVal / (halfY - offset));
            float depth = xVal + yVal;
            data.push_back(depth);
        } 
    }
    return data;
}