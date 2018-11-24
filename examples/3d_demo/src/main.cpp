#include <Core/common.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

const int WIDTH = 400;
const int HEIGHT = 400;

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
	/*scene.addObject(rotatingCamera, Size{ -1.0f, -1.0f, 0.0f, .5f, .5f, 0.0f });
	{
		SceneCameraConfig scConfig = scene.getCameraConfig(rotatingCamera);
		scConfig.dLightVisible = true;
		scConfig.pLightVisible = false;
		scConfig.sLightVisible = false;
		scene.configureCamera(rotatingCamera, scConfig);
	}*/
	
	
	
	Drawable cylinder, cube, sphere;
	{
		cylinder.setModel(primitives::generateCylinder(0.5f, 1.0f, 20.0f, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)));
		cylinder.setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
		scene.addObject(cylinder);
	}
	{
		//cube.setModel(primitives::generateQuad(1.0f, 1.0f, 1.0f, glm::vec4(0.3f, 0.8f, 0.3f, 1.0f)));
		cube.setModel(Model("C:/Users/Vincent/Documents/Projects/Blender/Example/vehicle2.fbx"));
		cube.setPosition(glm::vec3(-1.0f, 1.0f, -1.0f));
		cube.rotate(glm::radians(-90.0f), 0.0f, 0.0f);
		scene.addObject(cube);
	}
	{
		sphere.setModel(primitives::generateSphere(10, 15, glm::vec4(1.0f, 0.2f, 0.8f, 1.0f)));
		sphere.setPosition(glm::vec3(1.0f, 1.0f, 1.0f));
		scene.addObject(sphere);
	}

	Drawable ground;
	ground.setModel(primitives::generateQuad(20.0f, 0.2f, 20.0f, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f)));
	ground.setPosition(glm::vec3(0, -0.1f, 0));
	scene.addObject(ground);

	
	DirectionalLight sunLight;
	sunLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	sunLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	sunLight.change_direction(glm::vec3(1.0f, -1.0f, 1.0f));
	sunLight.intensity = 0.2f;
	sunLight.draw_shadow = true;
	scene.addObject(sunLight);
	

	PointLight poleLight;
	poleLight.setPosition(glm::vec3(2.0f, 2.0f, 0.0f));
	poleLight.radius = 6.0;
	poleLight.diffuse = glm::vec3(1.0f, 0.7f, 0.0f);
	poleLight.specular = glm::vec3(0.3f, 0.3f, 1.0f);
	poleLight.intensity = 1.0f;
	poleLight.setModel(primitives::generateQuad(0.3f, 0.3f, 0.3f, glm::vec4(0.0f, 0.0, 0.0f, 1.0f)));
	poleLight.draw_shadow = true;
	scene.addObject(poleLight);
	

	SpotLight flashLight;
	flashLight.attenuationLinear = 1.0;
	flashLight.attenuationQuadratic = 0.1;
	flashLight.cutOff = glm::radians(10.0f);
	flashLight.outerCutOff = glm::radians(20.0f);
	flashLight.diffuse = glm::vec3(0.8, 0.8, 0.2);
	flashLight.setPosition(glm::vec3(0.0, 10.0, 0.0));
	flashLight.direction = glm::vec3(0.0, -1.0, 0.0);
	flashLight.intensity = 1.0;
	flashLight.draw_shadow = true;
	scene.addObject(flashLight);
	


	sf::Clock clock;
	sf::Time deltaTime = clock.getElapsedTime();
	float delta = 0.0f;
	while (window.isOpen()) {
		clearScreen(glm::vec4(0.3f, 0.3f, 1.0f, 1.0f));
		
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed ||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				window.close();
				break;
			}
			if (e.type == sf::Event::Resized) {
				sf::Vector2u size = window.getSize();
				scene.resize(size.x, size.y);
				camera.Width = size.x;
				camera.Height = size.y;
			}
		}
		float millis = clock.getElapsedTime().asMilliseconds() * 0.001f;

		
		//rotate camera around scene		
		rotatingCamera.setPosition(glm::vec3(10.0f * cos(millis), 5.0f, 10.0f * sin(millis)));
		rotatingCamera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
		
		flashLight.direction = glm::vec3(glm::vec3(0.2f * cos(millis), -1.0f, 0.2f * sin(millis)));
		poleLight.intensity = sin(millis * 2) * 0.5 + 1.0;

		cube.rotate(1.0f * millis, 0.0f, 0.0f);
		float scale = abs(cos(millis));
		cube.scale(scale, scale, scale);


		sunLight.change_direction(glm::vec3(1.0f * cos(millis), -1.0f, 1.0f * sin(millis)));
		
		scene.update(delta);
		window.display();

		//calculate delta
		delta = (clock.getElapsedTime() - deltaTime).asMilliseconds();
		deltaTime = clock.getElapsedTime();
	}

}