
#include <iostream>
#include <ctime>
#include <math.h>

#include "Core/common.h"

#include "car.h"


GLfloat deltaTime = 0.0f, mouseX = 0, mouseY = 0, mousePitch = 0, mouseYaw = 0, mouseRoll = 0;

OrthographicCamera* camera;


void handle_key(Window &window);


int main() {

	WindowInfo wInfo;
	wInfo.maximized = false;
	wInfo.cursorLeave = false;
	Window window(wInfo, 1000, 1000, "Test123");

	Size windowSize = window.getSize();

	initCore();

	camera = new OrthographicCamera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	camera->FarZ = 15.0f;
	camera->NearZ = 0.0f;
	camera->Width = 20;
	camera->Height = 20;

	Scene scene(windowSize.width, windowSize.height);
	scene.addCamera(*camera, Size{ -1.0f, -1.0f, 0.0f, 2.0f, 2.0f, 0.0f });
	{
		SceneCameraConfig config = scene.getCameraConfig(*camera);
		config.dLightVisible = false;
		config.pLightVisible = false;
		config.sLightVisible = false;
		scene.configureCamera(*camera, config);
	}
	

	Car car;
	car.addToScene(scene);
	
	
	Drawable ground;
	ground.setModel(primitives::generateQuad(10.0f, 1.0f, 10.0f, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f)));
	ground.setPositionCenter(glm::vec3(0.0f, -0.5f, 0.0f));
	scene.addDrawable(ground);


	RigidBody groundRigidBody;
	Drawable anchor;
	anchor.setPositionCenter(glm::vec3(0.0f, -0.5f, 0.0f));
	collision::CollisionShape groundCollisonShape = collision::CollisionShape(collision::generateCube(ground.getSize()));
	groundRigidBody = RigidBody(groundCollisonShape, ground.getPositionCenter(), ground.getRotation(), 0.0f, false);
	groundRigidBody.setDrawable(ground);
	groundRigidBody.syncBody();
	scene.addRigidBody(groundRigidBody, 1, 2);


	sf::Clock clock;
	GLfloat lastTime = 0.00f;
	deltaTime = 0.0f;
	float fpsDelay = 1.0f, fpsCounter = fpsDelay;

	while (window.getWindow()->isOpen())
	{
		clearScreen(glm::vec4(0.4f, 0.4f, 0.7f, 1.0f));
		handle_key(window);

		scene.updatePhysics(deltaTime);


		scene.draw(deltaTime);

		window.getWindow()->display();
		GLfloat currentTime = clock.getElapsedTime().asMilliseconds();
		deltaTime = 0.001f * (currentTime - lastTime);
		lastTime = currentTime;
	}
	return 0;
}


void handle_key(Window &window) {	
	sf::Event e;
	while (window.getWindow()->pollEvent(e))
	{
		if (e.type == sf::Event::Closed) {
			window.getWindow()->close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.getWindow()->close();
		}
	}
}