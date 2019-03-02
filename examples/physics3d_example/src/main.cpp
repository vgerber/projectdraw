#include "Core/common.h"
#include "Physics/Physics3D/World/discreteworld.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "vehicle.h"

const int WIDTH  = 700;
const int HEIGHT = 700;

sf::Window * activeWindow = nullptr;
sf::Clock windowClock;
float deltaTime = 0.0f;

//returns false if activeWindow is closed
bool handleEvent(Camera &camera, Scene &scene);

std::vector<float> getHeightField(int width,  int length, float frequency = 0.1f, float amplitude = 1.5f);

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
    freeCamera.setClipping(freeCamera.getClippingNear(), 120.0f);
    freeCamera.setPosition(glm::vec3(-20.0f, 0.0f, 10.0f));
    freeCamera.setForward(glm::vec3(0.0f, 0.0f, 0.0f) - freeCamera.getPosition());
    mainScene.addObject(freeCamera);
	
    Mesh ground = *pd::generateQuad(40.0f, 40.0f, 0.2f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    ground.setPosition(glm::vec3(0.0f, 0.0f, -0.1f));
    mainScene.addObject(ground);

	Mesh testObject;
    testObject = *pd::generateSphere(20, 20,  glm::vec4(0.4f, 0.8f, 0.4f, 1.0f));
    testObject.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    testObject.scale(glm::vec3(4.0));
    mainScene.addObject(testObject);

    DirectionalLight sunLight;
    sunLight.changeDirection(glm::vec3(-0.3, -1.0f, -1.0));
    sunLight.diffuse = glm::vec3(1.0f, 1.0f, 0.8f);
    sunLight.ambient = sunLight.diffuse * 0.5f;
    sunLight.shadow = true;
    sunLight.intensity = 1.0f;
    mainScene.addObject(sunLight);
    
    
    
    DiscreteWorld physicsWorld;
    //mainScene.addObject(*physicsWorld.getDebugDrawable());

    collision::CollisionShape rTestBodyShape(collision::generateSphere(0.5f));
    RigidBody rTestBody(rTestBodyShape, 1000.0f, RigidType::DYNAMIC);
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

    Geometry triggerIndicator;
    mainScene.addObject(triggerIndicator);
    triggerIndicator.color = glm::vec4(0.0, 1.0, 1.0, 1.0);
    triggerIndicator.lineThickness = 10.0;
    triggerIndicator.settings.drawType = DrawType::LINEG;
    triggerIndicator.line(glm::vec3(0.0, 0.0, -2.5), glm::vec3(0.0, 0.0, 2.5));

    srand(time(0));
    collision::CollisionShape tBoxShape(collision::generateCube(Size(0.0, 0.0, 0.0, 3.0, 3.0, 5.0)));
    TriggerVolume tTestTrigger(tBoxShape, glm::vec3(0.0, 0.0, 2.5));
    triggerIndicator.setPosition(tTestTrigger.getPosition());
    tTestTrigger.onCollide = [&tTestTrigger, &triggerIndicator](const CollisionObject * collisionObject) {
        if(const RaycastVehicle * vehicle = dynamic_cast<const RaycastVehicle*>(collisionObject)) {
            glm::vec3 newPosition(
                ((float)rand() / RAND_MAX) * 50.0f -25.0f,
                ((float)rand() / RAND_MAX) * 10.0f -5.0f,
                2.5
            );
            triggerIndicator.setPosition(newPosition);
            tTestTrigger.setPosition(newPosition);
        }
    };
    physicsWorld.addCollisionObject(tTestTrigger);

    DemoVehicle vehicle1(physicsWorld, mainScene);
    vehicle1.getVehicleHandle()->getDrawable()->addChild(&freeCamera);

    int width = 100;
    int height = 100;
    std::vector<float> heightData = getHeightField(width, height);

    Mesh terrain;
    terrain = *pd::generateHeightfield(width, height, heightData);
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
        clearScreen(glm::vec4(0.8f, 0.8f, 1.0f, 1.0f));

		float deltaTimeMilli = deltaTime * 0.001f;

		//Keyboard events
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			rGroundBody.setPosition(ground.getPosition() + glm::vec3(0.0f, 0.0f, -5.0f * deltaTimeMilli));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			rGroundBody.setPosition(ground.getPosition() + glm::vec3(0.0f, 0.0f, 5.0f * deltaTimeMilli));
		}

        RaycastHitResult hitResult = physicsWorld.rayCastAll(glm::vec3(0.0, 10.0, 2.0), glm::vec3(0.0, -10.0, 2.0));

        //update all elements
        vehicle1.update(deltaTimeMilli);
        physicsWorld.update(std::max(deltaTime * 0.001f, 0.001f));
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
			camera.setSize(size.x, size.y);
		}
    }

    return true;
}

std::vector<float> getHeightField(int width, int height, float frequency, float amplitude) {
    std::vector<float> data;

    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            float xVal = sin(x * frequency) * amplitude;
            float yVal = sin(y * frequency) * amplitude;
            float depth = xVal * yVal;
            data.push_back(depth);
        } 
    }
    return data;
}