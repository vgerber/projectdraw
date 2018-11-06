#include <Core/common.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

const int WIDTH = 800;
const int HEIGHT = 800;

int main() {

	sf::ContextSettings settings;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.antialiasingLevel = 4;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "3D Demo", sf::Style::Default, settings);
	initCore();

	Scene scene(WIDTH, HEIGHT);

	Drawable cube;
	cube.setModel(primitives::generateQuad(1.0f, 1.0f, 1.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)));
	cube.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	scene.addObject(cube);
	Drawable ground;
	ground.setModel(primitives::generateQuad(5.0f, 0.5f, 5.0f, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f)));
	ground.setPosition(glm::vec3(-2.5f, -0.5f, -2.5f));
	scene.addObject(ground);



	PerspectiveCamera camera;
	camera.setPosition(glm::vec3(10.0f, 10.0f, 10.0f));
	camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	camera.Width = 10.0f;
	camera.Height = 10.0f;
	camera.FarZ = 30.0f;
	scene.addObject(camera, Size{ -1.0f, -1.0f, 0.0f, 2.0f, 2.0f, 0.0f });
	{
		SceneCameraConfig scConfig = scene.getCameraConfig(camera);
		scConfig.dLightVisible = true;
		scConfig.pLightVisible = false;
		scConfig.sLightVisible = false;
		scene.configureCamera(camera, scConfig);
	}

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
		camera.setPosition(glm::vec3(10.0f * cos(millis), 5.0f, 10.0f * sin(millis)));
		camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
		printf("Camera %f %f %f %f %f\n", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z, millis, delta);

		scene.update(delta);
		window.display();

		//calculate delta
		delta = (clock.getElapsedTime() - deltaTime).asMilliseconds();
		deltaTime = clock.getElapsedTime();
	}

}