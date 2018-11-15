#include <Core/common.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

const int WIDTH = 800;
const int HEIGHT = 800;

int main() {

	sf::ContextSettings settings;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "3D Demo", sf::Style::Default, settings);
	initCore();

	Scene scene(WIDTH, HEIGHT);
	   

	
	PerspectiveCamera camera;
	camera.setPosition(glm::vec3(5.0f, 5.0f, 5.0f));
	camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	camera.FarZ = 30.0f;
	camera.Width = WIDTH;
	camera.Height = HEIGHT;
	scene.addObject(camera);
	{
		SceneCameraConfig scConfig = scene.getCameraConfig(camera);
		scConfig.dLightVisible = true;
		scConfig.pLightVisible = false;
		scConfig.sLightVisible = false;
		scene.configureCamera(camera, scConfig);
	}
	
	PerspectiveCamera rotatingCamera;
	rotatingCamera.setPosition(glm::vec3(0.0f, 5.0f, -5.0f));
	rotatingCamera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	rotatingCamera.FarZ = 30.0f;
	rotatingCamera.Width = WIDTH;
	rotatingCamera.Height = HEIGHT;
	scene.addObject(rotatingCamera, Size{ -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f });
	{
		SceneCameraConfig scConfig = scene.getCameraConfig(rotatingCamera);
		scConfig.dLightVisible = true;
		scConfig.pLightVisible = false;
		scConfig.sLightVisible = false;
		scene.configureCamera(rotatingCamera, scConfig);
	}
	

	Drawable cube;
	//cube.setModel(primitives::generateQuad(1.0f, 1.0f, 1.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)));
	//cube.setModel(primitives::generateSphere(10, 10, glm::vec4(1.0f, 0.0f, 0.0, 1.0f)));
	cube.setModel(primitives::generateCylinder(0.5f, 1.0f, 20.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	cube.setPositionCenter(glm::vec3(0.0f, 0.5f, 0.0f));
	scene.addObject(cube);
	Drawable ground;
	ground.setModel(primitives::generateQuad(5.0f, 0.5f, 5.0f, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f)));
	ground.setPosition(glm::vec3(-2.5f, -0.5f, -2.5f));
	scene.addObject(ground);


	DirectionalLight sunLight;
	sunLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	sunLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	sunLight.change_direction(glm::vec3(1.0f, -1.0f, 1.0f));
	sunLight.intensity = 4.0f;
	sunLight.draw_shadow = true;
	scene.addObject(sunLight);


	sf::Clock clock;
	sf::Time deltaTime = clock.getElapsedTime();
	float delta = 0.0f;
	while (window.isOpen()) {
		clearScreen(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
			if (e.type == sf::Event::Resized) {
				sf::Vector2u size = window.getSize();
				scene.resize(size.x, size.y);
				camera.Width = size.x;
				camera.Height = size.y;
			}
		}

		
		//rotate camera around scene
		float millis = clock.getElapsedTime().asMilliseconds() * 0.001f;
		//rotatingCamera.setPosition(glm::vec3(10.0f * cos(millis), 5.0f, 10.0f * sin(millis)));
		//rotatingCamera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

		sunLight.change_direction(glm::vec3(1.0f * cos(millis), -1.0f, 1.0f * sin(millis)));
		
		scene.update(delta);
		window.display();

		//calculate delta
		delta = (clock.getElapsedTime() - deltaTime).asMilliseconds();
		deltaTime = clock.getElapsedTime();
	}

}