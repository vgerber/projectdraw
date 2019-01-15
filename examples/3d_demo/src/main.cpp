#include <Core/common.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

const int WIDTH = 400;
const int HEIGHT = 400;

float bankingAngle = 0.0f;

void moveDrawable(Drawable &drawable, Camera &camera, float delta);

int main() {

	sf::ContextSettings settings;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.depthBits = 24;
	settings.antialiasingLevel = 4;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "3D Demo", sf::Style::Default, settings);
	initCore();

	

	Scene scene(WIDTH, HEIGHT);
	   

	
	PerspectiveCamera camera;
	camera.setClipping(camera.getClippingNear(), 200.0f);
	camera.setSize(WIDTH, HEIGHT);
	camera.setPosition(glm::vec3(-4.0f, 0.0f, 2.0f));
	{
		Rotator camRotator;
		camRotator.rotateAxis(glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		camRotator.vectorRotation(camera.getForward(), -glm::normalize(camera.getPosition()));
		glm::vec3 euler = camRotator.getRotationEuler();
		euler.x = 0.0;
		camRotator.rotateEuler(euler);
		//camera.rotate(camRotator);
	}
	//camera.setForward(glm::vec3(0.0f, 0.0f, 0.0f) - camera.getPosition());
	
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
	rotatingCamera.setForward(glm::vec3(0.0f, 0.0f, 0.0f) - rotatingCamera.getPosition());
	rotatingCamera.setClipping(rotatingCamera.getClippingNear(), 30.0f);
	rotatingCamera.setSize(WIDTH, HEIGHT);
	/*scene.addObject(rotatingCamera, Size{ -1.0f, -1.0f, 0.0f, .5f, .5f, 0.0f });
	{
		SceneCameraConfig scConfig = scene.getCameraConfig(rotatingCamera);
		scConfig.dLightVisible = true;
		scConfig.pLightVisible = false;
		scConfig.sLightVisible = false;
		scene.configureCamera(rotatingCamera, scConfig);
	}*/
	
	
	
	Mesh quad, cube, sphere;
	{
		//cylinder.setModel(primitives::generateCylinder(0.5f, 1.0f, 20.0f, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)));
		quad = *pd::generateQuad(2.0f, 1.0f, 1.0f, glm::vec4(0.5f, 1.0f, 0.4f, 1.0f));
		//cylinder.setModel(primitives::generateSphere(10, 15, glm::vec4(1.0f, 0.2f, 0.8f, 1.0f)));
		quad.setPosition(glm::vec3(-1.0f, 0.0f, 2.0f));
		quad.settings.outlineVisible = true;
		quad.settings.outlineColor = glm::vec4(0.7, 0.7, 0.2, 1.0);
		quad.settings.outlineThickness = 0.03;
		quad.settings.xrayVisible = true;
		quad.settings.xrayCustomColor = true;
		quad.settings.xrayColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
		quad.settings.xrayDrawType = DrawType::LINEG;
		scene.addObject(quad);
		quad.addChild(&camera);
	}

	Transform wheelBaseTransform;
	{
#ifdef _WIN32
		cube = Mesh("C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/examples/assets/basic_car.fbx");
#elif linux
		cube = Mesh("/home/vincent/Development/Cpp/projectdraw_slim/examples/assets/basic_car.fbx");
#endif
		//cube.setPosition(glm::vec3(-1.0f, -1.0f, 1.0f));
		wheelBaseTransform = (static_cast<Mesh*>(cube.getChild("WheelFrontR")))->getTransform();
		cube.settings.normalVisible = true;
		scene.addObject(cube);
	}
	{
		//sphere.setModel(primitives::generateSphere(10, 15, glm::vec4(1.0f, 0.2f, 0.8f, 1.0f)));
		sphere = *pd::generateQuad(0.3f, 0.3f, 0.3f, glm::vec4(0.8f, 0.5f, 0.8f, 1.0f));
		sphere.setPosition(glm::vec3(1.0f, 1.0f, 1.0f));
		scene.addObject(sphere);
	}

	Mesh ground;
	ground = *pd::generateQuad(20.0f, 20.0f, 0.2f, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
	ground.setPosition(glm::vec3(0, 0.0f, -0.1f));
	scene.addObject(ground);


	
	DirectionalLight sunLight;
	sunLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	sunLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	sunLight.changeDirection(glm::vec3(1.0f, 1.0f, -1.0f));
	sunLight.intensity = 0.1f;
	sunLight.shadow = false;
	scene.addObject(sunLight);
	

	PointLight poleLight;
	poleLight.setPosition(glm::vec3(2.0f, 0.0f, 2.0f));
	poleLight.setDistance(20.0);
	poleLight.diffuse = glm::vec3(0.01f, 0.01f, 1.0f);
	poleLight.specular = poleLight.diffuse;
	poleLight.intensity = 1.0f;
	//poleLight.setModel(primitives::generateQuad(0.3f, 0.3f, 0.3f, glm::vec4(0.0f, 0.0, 0.0f, 1.0f)));
	poleLight.shadow = false;
	scene.addObject(poleLight);

	PointLight poleLight2;
	poleLight2.setPosition(glm::vec3(-2.0f, 0.0f, 2.0f));
	poleLight2.setDistance(20.0);
	poleLight2.diffuse = poleLight.diffuse;
	poleLight2.specular = poleLight2.diffuse;
	poleLight2.intensity = 1.0f;
	//poleLight2.setModel(primitives::generateQuad(0.3f, 0.3f, 0.3f, glm::vec4(0.0f, 0.0, 0.0f, 1.0f)));
	poleLight2.shadow = false;
	scene.addObject(poleLight2);
	
	
	SpotLight flashLight;
	flashLight.attenuationLinear = 1.0;
	flashLight.attenuationQuadratic = 0.1;
	flashLight.cutOff = glm::radians(10.0f);
	flashLight.outerCutOff = glm::radians(20.0f);
	flashLight.diffuse = glm::vec3(0.8, 0.8, 0.2);
	flashLight.specular = flashLight.diffuse;
	flashLight.setPosition(glm::vec3(0.0, 0.0, 10.0));
	flashLight.direction = glm::vec3(0.0, -1.0, 0.0);
	flashLight.intensity = 0.6;
	flashLight.shadow = false;
	scene.addObject(flashLight);

	SpotLight flashLight2;
	flashLight2.attenuationLinear = 1.0;
	flashLight2.attenuationQuadratic = 0.1;
	flashLight2.cutOff = glm::radians(20.0f);
	flashLight2.outerCutOff = glm::radians(30.0f);
	flashLight2.diffuse = glm::vec3(0.8, 0.8, 0.2);
	flashLight2.specular = flashLight2.diffuse;
	flashLight2.setPosition(glm::vec3(-3.0, 2.0, 3.0));
	flashLight2.direction = glm::normalize(-flashLight2.getPosition());
	flashLight2.intensity = 0.3;
	flashLight2.shadow = false;
	scene.addObject(flashLight2);
	

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
				camera.setSize(size.x, size.y);
			}
		}
		float millis = clock.getElapsedTime().asMilliseconds() * 0.001f;

		moveDrawable(quad, camera, delta);
		
		//rotate camera around scene		
		rotatingCamera.setPosition(glm::vec3(10.0f * cos(millis), 5.0f, 10.0f * sin(millis)));
		rotatingCamera.setForward(glm::vec3(0.0f, 0.0f, 0.0f) - rotatingCamera.getPosition());
		
		flashLight.direction = glm::vec3(0.2f * cos(millis), 0.2f * sin(millis), -1.0f);
		
		poleLight.intensity = sin(millis * 10) * 0.5 + 1.0;
		poleLight2.intensity = sin(millis * 10 + 1.6) * 0.5 + 1.0;

		sunLight.changeDirection(glm::vec3(1.0f * cos(millis), 1.0f * sin(millis), -1.0f));

		 {
			Drawable * backWheel = static_cast<Drawable*>(cube.getChild("WheelFrontR"));
			//backWheel->setPosition(sin(millis * 1.0f), cos(millis * 1.0f), 0.0f);
			//wheelBaseTransform.print();

			Rotator rotator(glm::radians(-millis * 100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			rotator.applyRotation(Rotator(glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
			backWheel->rotate(rotator);
			backWheel->setPosition(wheelBaseTransform.getTranslation() + glm::vec3(0.0f, 0.0f, 2.0f));
			cube.setPosition(0.0f, 1.0f, 2.0f);
		 }

		{
			Rotator rotator;
			rotator.rotateAxis(glm::radians(bankingAngle), quad.getForward());
			Rotator quadRotator; // = quad.getRotator();
			quadRotator.vectorRotation(quad.getBaseForward(), quad.getForward());
			quadRotator.applyRotation(rotator);
			quad.rotate(quadRotator);

		}		
		{
			Rotator rotator, rotator2, rotator3;
			sphere.setPosition(quad.getPosition() + glm::vec3(1.5f, 1.5f, 0.0f));
			rotator.setOrigin(quad.getPosition() - sphere.getPosition());
			rotator.rotateAxis(glm::radians(millis * 200.0f), glm::vec3(0.0f, 0.0f, 1.0f));

			rotator2.rotateAxis(glm::radians(millis * 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));

			rotator3.rotateAxis(glm::radians(millis * 50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			rotator3.applyRotation(rotator2);
			rotator3.applyRotation(rotator);

			sphere.rotate(rotator3);
		}

		scene.update(delta);
		window.display();

		//calculate delta
		delta = (clock.getElapsedTime() - deltaTime).asMilliseconds();
		deltaTime = clock.getElapsedTime();
	}

}


void moveDrawable(Drawable &drawable, Camera &camera, float delta) {
	float camSpeed = 0.01f;;
	float speed = 0.004f;
	float steering = 0.004f;
	float maxBanking = 30.0f;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		glm::vec3 move = drawable.getForward();
		drawable.setPosition(drawable.getPosition() + move * delta * speed);
		bankingAngle = 0.0;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		glm::vec3 move = -drawable.getForward();
		drawable.setPosition(drawable.getPosition() + move * delta * speed);
		bankingAngle = 0.0;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		Rotator baseRotator = drawable.getRotator();
		Rotator steerRotator;
		steerRotator.rotateAxis(glm::radians(0.1f) * delta, glm::vec3(0.0f, 0.0f, 1.0f));
		baseRotator.applyRotation(steerRotator);
		drawable.rotate(baseRotator);
		bankingAngle = -maxBanking; // std::max(bankingAngle - maxBanking * delta * steering, -maxBanking);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		Rotator baseRotator = drawable.getRotator();
		Rotator steerRotator;
		steerRotator.rotateAxis(-glm::radians(0.1f) * delta, glm::vec3(0.0f, 0.0f, 1.0f));
		baseRotator.applyRotation(steerRotator);
		drawable.rotate(baseRotator);
		bankingAngle = maxBanking;//std::min(bankingAngle + maxBanking * delta * steering, maxBanking);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		glm::vec3 move = drawable.getUp();
		drawable.setPosition(drawable.getPosition() + move * delta * speed);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		glm::vec3 move = drawable.getUp();
		drawable.setPosition(drawable.getPosition() - move * delta * speed);
	}
	/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
		glm::vec3 rot = glm::vec3(1.0f, 0.0f, 0.0f);
		drawable.rotate(drawable.getRotation() + rot * delta * speed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
		glm::vec3 rot = glm::vec3(0.0f, 1.0f, 0.0f);
		drawable.rotate(drawable.getRotation() + rot * delta * speed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
		glm::vec3 rot = glm::vec3(0.0f, 0.0f, 1.0f);
		drawable.rotate(drawable.getRotation() + rot * delta * speed);
	}
	*/

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		camera.setPosition(camera.getPosition() + camera.getUp() * delta * camSpeed);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		camera.setPosition(camera.getPosition() + -camera.getUp() * delta * camSpeed);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		camera.setPosition(camera.getPosition() + camera.getRight() * delta * camSpeed);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		camera.setPosition(camera.getPosition() + -camera.getRight() * delta * camSpeed);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
		camera.setPosition(camera.getPosition() + camera.getForward() * delta * camSpeed);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
		camera.setPosition(camera.getPosition() - camera.getForward() * delta * camSpeed);
	}
}