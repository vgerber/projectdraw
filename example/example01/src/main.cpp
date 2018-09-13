
#include <iostream>
#include <ctime>
#include <math.h>

#include "Core/common.h"

#ifdef _WIN32
std::string path_obj_mountain = "C:/Users/Vincent/Documents/Projects/Blender/TriFace/basic_mountain.obj";
#endif // _WIN32
#ifdef linux
std::string path_obj_mountain = "/home/vincent/Development/Cpp/opengl/basic_mountain.obj";
#endif // linux


GLfloat deltaTime = 0.0f, mouseX = 0, mouseY = 0, mousePitch = 0, mouseYaw = 0, mouseRoll = 0;


bool initialCameraMove = true;
PerspectiveCamera mainCamera;
OrthographicCamera testCamera;

Geometry *carTrace = nullptr;

btRaycastVehicle *vehicle = nullptr;
Vehicle *testVehicle = nullptr;

Scene *mainScenePtr;

int sceneCamera = 0;

/*
void key_callback(sf::Window *window, int key, int scancode, int action, int mode);
void mouse_callback(sf::Window *window, double xpos, double ypos);
void mouse_scroll_callback(sf::Window *window, double xoffset, double yoffset);*/
void handle_key(Window &window);

bool emergencyBrake = false;

int main() {

	WindowInfo wInfo;
	wInfo.maximized = false;
	wInfo.cursorLeave = false;
	Window window(wInfo, 800, 600, "Test123");

	Size windowSize = window.getSize();

	init_core();
		

	std::vector<std::string> skybox_faces;
	//create camera
	mainCamera.setPosition(glm::vec3(5.0f, 15.0f, 5.0f));
	mainCamera.FarZ = 500.0f;
	mainCamera.Width = windowSize.width;
	mainCamera.Height = windowSize.height;
	
	PerspectiveCamera carCamera;
	carCamera.FarZ = 100.0f;
	carCamera.Width = windowSize.width;
	carCamera.Height = windowSize.height;


	testCamera = OrthographicCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	testCamera.setPosition(glm::vec3(1.0f, 20.0f, 1.0f));
	testCamera.lookAt(glm::vec3(1.0f, 0.0f, 1.0f));
	testCamera.FarZ = 65.0f;
	testCamera.NearZ = 0.1f;
	testCamera.Height = 200 * (windowSize.height / windowSize.width);
	testCamera.Width = 200;


	/*
	for (glm::vec3 point : localViewFrustum.nearCorners) {
		geoLight.addPoint(point);
	}*/

	//
	// Light
	//
	DirectionalLight dLight = DirectionalLight();
	//dLight.direction = glm::vec3(-2.0f, -2.0f, -2.0f);
	dLight.ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	dLight.diffuse = glm::vec3(1.0f, 0.7f, 0.5f);
	dLight.specular = dLight.diffuse;
	dLight.intensity = .3f;
	dLight.change_direction(glm::vec3(-1.0f, -0.4f, -1.0f));

	glm::vec3 light_color(0.0f, 0.0f, 1.0f);
	glm::vec3 light_spec(0.0f, 0.0f, 1.0f);

	PointLight pLightLeft;
	pLightLeft.intensity = 1.0f;
	pLightLeft.setPosition(glm::vec3(4.0f, 4.0f, 0.0f));
	pLightLeft.ambient = glm::vec3(0.001f, 0.01f, 0.001f);
	pLightLeft.diffuse = light_color;
	pLightLeft.specular = light_spec;
	pLightLeft.radius = 200.0f;


	PointLight pLightRight;
	pLightRight.intensity = 1.0f;
	pLightRight.setPosition(glm::vec3(4.0f, 4.0f, 0.0f));
	pLightRight.ambient = glm::vec3(0.001f, 0.01f, 0.001f);
	pLightRight.diffuse = light_color;
	pLightRight.specular = light_spec;
	pLightRight.radius = 200.0f;

	light_color = glm::vec3(1.0f, 1.0f, 1.0f);
	light_spec  = glm::vec3(1.0f, 1.0f, 1.0f);
	PointLight pLight2;
	pLight2.intensity = 1.f;
	pLight2.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	pLight2.ambient = glm::vec3(0.001f, 0.01f, 0.001f);
	pLight2.diffuse = light_color;
	pLight2.specular = light_spec;
	pLight2.radius = 300.0f;
	
	SpotLight frontLight;
	frontLight.intensity = 0.6f;
	frontLight.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	frontLight.setDistance(200.0f);
	frontLight.setCutOff(12.0, 20.0);


    Drawable test_obj = Drawable(&path_obj_mountain[0]);

	
	//
	// TEXT FreeType
	//
	Text text_fps(Loader::GetPath("/Fonts/VeraMono.ttf").c_str(), 60);
	Shaders[SHADER_FONT].use();
	text_fps.scale(0.01f, 0.01f, 0.01f);
	glUniformMatrix4fv(glGetUniformLocation(Shaders[SHADER_FONT].getId(), "model"), 1, GL_FALSE, glm::value_ptr(text_fps.getModelMatrix()));

	//
	// 
	//


	Scene scene_main(windowSize.width, windowSize.height);
	mainScenePtr = &scene_main;


	Size camSize{ -1.0f, -1.0f, 0.0f, 2.0f, 2.0f, 0.0f };
	scene_main.addCamera(mainCamera, camSize);
	//camSize = { -1.0f, -1.0f, 0.0f, 2.0f, 2.0f, 0.0f };
	scene_main.addCamera(testCamera, camSize);
	SceneCameraConfig testCamConfig = scene_main.getCameraConfig(testCamera);
	testCamConfig.dLightVisible = false;
	testCamConfig.pLightVisible = false;
	testCamConfig.slightVisible = false;
	testCamConfig.ParticleVisible = false;
	scene_main.configureCamera(testCamera, testCamConfig);


	scene_main.setDlight(dLight);
	//dLight.change_direction(glm::vec3(-1.0f, 0.0f, 0.0f));
	dLight.draw_shadow = true;

	scene_main.addPlight(pLightLeft);
	scene_main.addPlight(pLightRight);

	scene_main.addPlight(pLight2);
	pLightLeft.setPosition(glm::vec3(2.0f, 1.0f, 0.0f));
	pLight2.setPosition(glm::vec3(5.0f, 1.0f, 0.0f));

	scene_main.addSLight(frontLight);

	scene_main.addDrawable(test_obj);

	Size size_small;
	size_small.height = 0.1f;
	size_small.width = 0.1f;
	size_small.depth = 0.1f;

	Size size_medium;
	size_medium.height = 1.0f;
	size_medium.width = 1.0f;
	size_medium.depth = 1.0f;

	Size size_big;
	size_big.height = 5.0f;
	size_big.width = 5.0f;
	size_big.depth = 5.0f;

	test_obj.scaleToHeight(50.0f);
	Size size = test_obj.getSize();
	test_obj.setPosition(glm::vec3(-250.0, -4.0f, -250.0f));
	//test_obj.setCenter(glm::vec3(0.5f, 0.5f, 1.7f));
	test_obj.settings.boxVisible =  false;
	test_obj.settings.drawType = DrawType::TRIANGLEG;

	pLightLeft.setModel(primitives::generate_quad(0.5f, 0.5f, 0.5f, glm::vec4(0.8f)));
	pLightRight.setModel(primitives::generate_quad(0.5f, 0.5f, 0.5f, glm::vec4(0.8f)));
	pLight2.setModel(pLightLeft.getModel());

	//cube.scaleToSize(size_medium);
	//cube.setPosition(glm::vec3(1.0f, 5.0f, 0.0f));	
	//cube.visibleNormal = true;

	std::vector<float> heightData = {
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 3.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	Drawable testHeightField;
	testHeightField.setModel(primitives::generateHeightfieldStep(10, 10, heightData, glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)));
	testHeightField.setPosition(glm::vec3(20.0f, 0.1f, 20.0f));
	testHeightField.settings.normalVisible = true;

	{
		Model *thfPtr = testHeightField.getModelPtr();
		BasicMesh mesh = thfPtr->getMeshes()[0];

		std::vector<Vertex> vertices = mesh.getVertices();
		for (Vertex &v : vertices) {
			v.Color = v.Position.y > 0.2f ? glm::vec4(0.5f, 0.1f, 0.1f, 1.0f) : glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
		}
		mesh.setVertices(vertices);
		thfPtr->setMesh(0, mesh);
	}

	scene_main.addDrawable(testHeightField);


	//Instancing 
	
	Model instancerModel = primitives::generate_quad(0.5f, 0.5f, 0.5f, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
	Instancer instancer(instancerModel, 1000);

	for (int i = 0; i < instancer.getModelMatrices().size(); i++) {

		glm::vec3 position;
		position.x += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 20.0f;
		position.y += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 20.0f;
		position.z += (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 200.0f;

		glm::mat4 mmatrix = glm::translate(glm::mat4(1.0f), position);
		instancer.setModelMatrix(mmatrix, i);
	}

	scene_main.addInstancer(instancer);


	Drawable test_rect = Drawable();
	test_rect.setModel(primitives::generateCircle(1.0f, 10.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	test_rect.setPosition(glm::vec3(0.0f, 5.0f, -2.0f));
	
	test_rect.settings.drawType = DrawType::TRIANGLEG;
	scene_main.addDrawable(test_rect);


	carTrace = new Geometry();
	scene_main.addDrawable(*carTrace);


	scene_main.addDrawable(text_fps);
	text_fps.rotate(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));
	text_fps.setColor(1.0f, 1.0f, 0.1f, 1.0f);
	text_fps.setPosition(glm::vec3(-1.0f, 1.0f, -2.0f));


	Text text_description = Text(Loader::GetPath("/Fonts/VeraMono.ttf").c_str(), 60);
	text_description.setPosition(glm::vec3(0.0f, 0.1f, -10.0f));
	text_description.rotate(glm::radians(glm::vec3(0.0f, -30.0f, 0.0f)));
	text_description.setColor(1.0f, 0.5f, 0.0f, 1.0f);
	text_description.setText("This is a great test level!");
	scene_main.addDrawable(text_description);

	std::vector<Drawable*> cubes;
	
	glm::vec3 cubes_position(0.0f, 0.5f, 15.0f);

	srand(static_cast <unsigned> (time(0)));
	for(size_t x = 0; x < 3; x++) {
		for(size_t y = 0;  y < 3; y++) {
			for(size_t z = 0; z < 3; z++) {
				Drawable cube;
				glm::vec4 cubeColor(
					static_cast<float>(rand() / static_cast<float>(RAND_MAX)), 
					static_cast<float>(rand() / static_cast<float>(RAND_MAX)), 
					static_cast<float>(rand() / static_cast<float>(RAND_MAX)), 
					1.0f);
				cube.setModel(primitives::generate_quad(1.f, 1.0f, 1.0f, cubeColor));
				cube.setPosition(cubes_position + glm::vec3(x * 1.0f, y * 1.0f, z * 1.0f));
				cubes.push_back(new Drawable(cube));
				scene_main.addDrawable(*cubes[cubes.size()-1]);
			}
		}
	}


	Drawable borderGround = Drawable();
	borderGround.setModel(primitives::generate_quad(500.0f, 5.0f, 500.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));

	Size sizeGround = borderGround.getSize();
	GLfloat heightScale = 2.0f;

	Drawable borderAnchor, borderBack, borderFront, borderLeft, borderRight;
	borderBack.setModel(primitives::generate_quad(sizeGround.width + 6.0f, sizeGround.height * heightScale, 5.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));
	borderFront.setModel(primitives::generate_quad(sizeGround.width + 6.0f, sizeGround.height * heightScale, 5.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));
	borderLeft.setModel(primitives::generate_quad(5.0f , sizeGround.height * heightScale, sizeGround.depth, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));
	borderRight.setModel(primitives::generate_quad(5.0f, sizeGround.height * heightScale, sizeGround.depth, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));

	scene_main.addDrawable(borderGround);
	scene_main.addDrawable(borderBack);
	scene_main.addDrawable(borderFront);
	scene_main.addDrawable(borderLeft);
	scene_main.addDrawable(borderRight);




	//
	// Vehicle
	//
	GLfloat carMass = 1000.0f;
	GLfloat carWheelThickness = 0.2f;

	Drawable carAnchor;
	carAnchor.setPositionCenter(glm::vec3(0.0f, 0.0f, 0.0f));
	Drawable carChassis;
	carChassis.settings.xrayVisible = false;
	carChassis.settings.xrayColor = glm::vec4(1.0f, 0.1f, 0.0f, 1.0f);
	carChassis.settings.outlineVisible = true;
	carChassis.settings.outlineThickness = 0.1f;
	carChassis.setModel(primitives::generate_quad(4.0f, 2.0f, 7.0f, glm::vec4(0.1f, 0.3f, 0.8f, 0.7f)));
	carChassis.setPositionCenter(carAnchor.getPositionCenter());

	std::vector<Drawable*> carWheels;
	for (int i = 0; i < 4; i++) {
		Drawable *wheel = new Drawable();
		wheel->setModel(primitives::generate_quad(carWheelThickness, 0.5, 0.5, glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)));
		carWheels.push_back(wheel);
	}

	{
		glm::vec3 carCenter = carChassis.getPositionCenter();
		Size carSize = carChassis.getSize();

		carWheels[0]->setPositionCenter(glm::vec3(carCenter.x + (carSize.width * 0.5f + carWheelThickness * 0.5f), carCenter.y - carSize.height * 0.5f, carCenter.z + carSize.depth * 0.3f));
		carWheels[1]->setPositionCenter(glm::vec3(carCenter.x - (carSize.width * 0.5f + carWheelThickness * 0.5f), carCenter.y - carSize.height * 0.5f, carCenter.z + carSize.depth * 0.3f));
		carWheels[2]->setPositionCenter(glm::vec3(carCenter.x + (carSize.width * 0.5f + carWheelThickness * 0.5f), carCenter.y - carSize.height * 0.5f, carCenter.z - carSize.depth * 0.3f));
		carWheels[3]->setPositionCenter(glm::vec3(carCenter.x - (carSize.width * 0.5f + carWheelThickness * 0.5f), carCenter.y - carSize.height * 0.5f, carCenter.z - carSize.depth * 0.3f));
	}

	text_fps.setPositionCenter(carChassis.getPositionCenter() + glm::vec3(0.0f, 2.0f, 0.0f));
	text_fps.setCenterInWorld(carChassis.getPositionCenter());
	text_fps.settings.boxVisible = true;

	pLightLeft.setPosition(carChassis.getPositionCenter() + glm::vec3(-carChassis.getSize().width * 0.5f + 0.25, carChassis.getSize().height * 0.5f, -carChassis.getSize().depth * 0.3f));
	pLightLeft.setCenterInWorld(carChassis.getPositionCenter());

	pLightRight.setPosition(carChassis.getPositionCenter() + glm::vec3(carChassis.getSize().width * 0.5f - 0.75f, carChassis.getSize().height * 0.5f, -carChassis.getSize().depth * 0.3f));
	pLightRight.setCenterInWorld(carChassis.getPositionCenter());

	Geometry carDistanceLine;
	{
		Size carSize = carChassis.getSize();
		glm::vec3 carFront(0.0f, 0.0f, -carSize.depth * 0.5f);
		carDistanceLine.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		carDistanceLine.line(carFront, carFront + glm::vec3(0.0f, 0.0f, -20.0f));
		
		carDistanceLine.settings.drawType = DrawType::LINEG;
		carDistanceLine.setCenterInWorld(carAnchor.getPosition());
	}

	Geometry carSelectedCircle;
	{
		Size carSize = carChassis.getSize();
		carSelectedCircle.lineTo(primitives::geometryCircle(carSize.depth * 0.6f, 30.0f, glm::vec4(0.2f, 1.0f, 0.2f, 1.0f)));
		carSelectedCircle.settings.drawType = DrawType::LINEG;
		carSelectedCircle.lineThickness = 5.0f;
		carSelectedCircle.rotate(glm::radians(90.0f), 0.0f, 0.0f);
	}


	scene_main.addDrawable(carDistanceLine);
	scene_main.addDrawable(carSelectedCircle);

	scene_main.addDrawable(carChassis);
	for (auto wheel : carWheels) {
		scene_main.addDrawable(*wheel);
	}




	//depth
	glEnable(GL_DEPTH_TEST);	
	glDepthFunc(GL_LESS);



	glEnable(GL_CULL_FACE);
	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_MULTISAMPLE);

	glEnable(GL_FRAMEBUFFER_SRGB);

	
	glEnable(GL_LINE_SMOOTH);
	
	//glEnable(GL_LINE_STIPPLE);	
	//glLineStipple(1, 0xAAAA);

	carTrace->settings.drawType = DrawType::LINEG;
	carTrace->lineThickness = 3;

	//cube.setPosition(glm::vec3(0.0f, 3.0f, 0.0f));
	
	//compensation log db exam?

	std::vector<RigidBody*> rigidBodys;

	borderAnchor.setPositionCenter(glm::vec3(0.0f, 0.0f, 0.0f));

	borderGround.setPositionCenter(borderAnchor.getPositionCenter());
	borderGround.rotate(glm::radians(glm::vec3(0.0f, 0.0f, 0.0f)));
	borderGround.settings.boxVisible = false;

	//calc positions in compound
	borderBack.setPosition(borderGround.getPosition() - glm::vec3(1.0f, 0.0f, 1.0f));
	borderBack.setCenterInWorld(borderGround.getPositionCenter());
	borderFront.setPosition(borderGround.getPosition() - glm::vec3(1.0f, 0.0f, -sizeGround.depth));
	borderFront.setCenterInWorld(borderGround.getPositionCenter());
	borderLeft.setPosition(borderGround.getPosition() - glm::vec3(1.0f, 0.0f, 0.0f));
	borderLeft.setCenterInWorld(borderGround.getPositionCenter());
	borderRight.setPosition(borderGround.getPosition() + glm::vec3(sizeGround.width, 0.0f, 0.0f));
	borderRight.setCenterInWorld(borderGround.getPositionCenter());


	//cube.setPositionCenter(glm::vec3(0.0f, 10.0f, 10.0f));
	//cube.rotate(glm::radians(glm::vec3(40.0f, 40.0f, 40.0f)));

	text_fps.setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	//add borderGround as borderGround plane
	collision::CollisionShape borderGroundShape(collision::generateCube(borderGround.getSize()));
	collision::CollisionShape borderFrontShape(collision::generateCube(borderFront.getSize()));
	collision::CollisionShape borderBackShape(collision::generateCube(borderBack.getSize()));
	collision::CollisionShape borderLeftShape(collision::generateCube(borderLeft.getSize()));
	collision::CollisionShape borderRightShape(collision::generateCube(borderRight.getSize()));
	{		
		btCompoundShape* borderCompound = new btCompoundShape();
		btTransform transform;
		transform.setIdentity();

		glm::vec3 posCenter = glm::vec3(0.0f, 0.0f, 0.0f);//borderGround.getPositionCenter();
		transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
		borderCompound->addChildShape(transform, borderGroundShape.getShape());
		
		posCenter = borderBack.getPositionCenter();
		transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
		borderCompound->addChildShape(transform, borderFrontShape.getShape());

		posCenter = borderFront.getPositionCenter();
		transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
		borderCompound->addChildShape(transform, borderBackShape.getShape());
		
		posCenter = borderLeft.getPositionCenter();
		transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
		borderCompound->addChildShape(transform, borderLeftShape.getShape());

		posCenter = borderRight.getPositionCenter();
		transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
		borderCompound->addChildShape(transform, borderRightShape.getShape());
		

		collision::CollisionShape borderCompoundShape(borderCompound);

		RigidBody rbody(borderCompoundShape, borderAnchor.getPositionCenter(), borderAnchor.getRotation(), 0.0f);
		rbody.getBody()->setFriction(btScalar(1.0f));
		rbody.setDrawable(borderAnchor);
		rbody.getBody()->setCollisionFlags(rbody.getBody()->getCollisionFlags() | btRigidBody::CF_KINEMATIC_OBJECT);
		rbody.getBody()->setActivationState(DISABLE_DEACTIVATION);
		rbody.visibleAABB = false;
		rigidBodys.push_back(new RigidBody(rbody));

		borderAnchor.setPositionCenter(glm::vec3(0.0f, -sizeGround.height * 0.5f, 0.0f));
		rbody.syncBody();

	}

	//set cube as sceond item
	{
		
		for(auto cube : cubes) {
			cube->scaleToHeight(1.0f);
			collision::CollisionShape cube_shape(collision::generateCube(cube->getSize()));
			RigidBody rbodyCube(cube_shape, cube->getPositionCenter(), cube->getRotation(), 80.0f);
			rbodyCube.setDrawable(*cube);
			rbodyCube.visibleAABB = false;
			rigidBodys.push_back(new RigidBody(rbodyCube));
		}
	}

	cubes[cubes.size() - 1]->scaleToHeight(0.5f);
	rigidBodys[rigidBodys.size() - 1]->syncBody();

	{
		collision::CollisionShape text_shape(collision::generateSphere(test_rect.getSize().width * 0.5));
		RigidBody rbody(text_shape, test_rect.getPositionCenter(), test_rect.getRotation(), 100.0f);
		rbody.setDrawable(test_rect);
		rigidBodys.push_back(new RigidBody(rbody));
	}

	//set vehicle physics
	Size carSize = carChassis.getSize();
	carSize.height = 0.1f;
	collision::CollisionShape carChassisShape = collision::CollisionShape(collision::generateCube(carSize));
	

	
	{
		btTransform transform;
		transform.setIdentity();

		//btCompoundShape *compoundShape = new btCompoundShape();
		//transform.setOrigin(toBtVec3(carAnchor.getPosition() + glm::vec3(0.0f, 0.5f * carChassis.getSize().height, 0.0f)));
		//compoundShape->addChildShape(transform, carChassisShape.getShape());
		//collision::CollisionShape carHullShape(compoundShape);
		
		RigidBody rbody(carChassisShape, carAnchor.getPositionCenter(), carAnchor.getRotation(), carMass);
		rbody.setDrawable(carAnchor);
		//transform.setOrigin(btVector3(0.01f, -0.5f * carChassis.getSize().height, 0.01f));
		//rbody.getBody()->setCenterOfMassTransform(transform);
		rbody.visibleAABB = false;
		rbody.setDrawable(carChassis);
		rigidBodys.push_back(new RigidBody(rbody));
		
		testVehicle = new Vehicle(new RigidBody(rbody), scene_main.getPhysicsWorld());

		btScalar connectionHeight(0.5f);
		btVector3 wheelConnectionPoint(carChassis.getSize().width * 0.6, connectionHeight, carChassis.getSize().depth * 0.3);
		btVector3 wheelConnectionPoints[] = {
			btVector3( 1, 1,  1),
			btVector3(-1, 1,  1),
			btVector3( 1, 1, -1),
			btVector3(-1, 1, -1)
		};
		
		for (int i = 0; i < carWheels.size(); i++) {
			Wheel wheel;
			wheel.drawable = carWheels[i];
			testVehicle->addWheel(wheel, toVec3(wheelConnectionPoint * wheelConnectionPoints[i]), (i > 1));
		}

		scene_main.addVehicle(*testVehicle);
		vehicle = testVehicle->getVehicle();


		carAnchor.setPositionCenter(glm::vec3(0.0f, carChassis.getSize().height * 1.0f, 0.0f));
		rbody.syncBody();

	}

	test_obj.scaleToHeight(100.0f);
	collision::CollisionShape terrainShape = collision::CollisionShape(collision::generateTriangleMesh(test_obj.getModel()));
	{
		RigidBody rbody(terrainShape, test_obj.getPositionCenter(), test_obj.getRotation(), 0.0f);
		rbody.setDrawable(test_obj);
		rbody.visibleAABB = true;
		rigidBodys.push_back(new RigidBody(rbody));
	}

	
	testHeightField.scaleToHeight(40.0f);
	
	collision::CollisionShape heightShape = collision::CollisionShape(collision::generateTriangleMesh(testHeightField.getModel()));
	{
		RigidBody rbody(heightShape, testHeightField.getPositionCenter(), testHeightField.getRotation(), 0.0f);
		rbody.setDrawable(testHeightField);
		rbody.syncBody();
		rbody.visibleAABB = true;
		rigidBodys.push_back(new RigidBody(rbody));

		testHeightField.setPositionCenter(glm::vec3(40.0f, 0.07f, 40.0f));
		rbody.syncBody();
	}
	
	


	for (auto body : rigidBodys) {
		scene_main.addRigidBody((*body));
	}

	Drawable particle;
	particle.setModel(primitives::generate_quad(1.0f, 1.0f, 1.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));
	ParticleGenerator pgTest(particle, 1000);
	scene_main.addParticleGenerator(pgTest);

	//window.getWindow()->setFramerateLimit(60);

	sf::Clock clock;
	GLfloat lastTime = 0.00f;
	deltaTime = 0.0f;
	while (window.getWindow()->isOpen())
	{
		if (sceneCamera == 0) {
			scene_main.enableCamera(mainCamera, true);
			scene_main.enableCamera(testCamera, false);
		}
		else {
			scene_main.enableCamera(mainCamera, false);
			scene_main.enableCamera(testCamera, true);
		}

		lastTime = clock.getElapsedTime().asMilliseconds();
		window.update();
		
		handle_key(window);
		
		
		
		
		//borderGround.setPositionCenter(glm::vec3(0.0f, 0.0f, 0.0f));
		/*if(lastTime > 0.0f) { 
			borderAnchor.rotate(glm::radians(glm::vec3(cos((lastTime) * 1.5f) * 0.0f, lastTime * 00.0f, cos((lastTime) * 1.0f) * 0.0f)));
			rigidBodys[0]->syncBody();
		}*/

		scene_main.updatePhysics(deltaTime);

		//synchronize walls to ground
		
		borderGround.setPositionCenter(borderAnchor.getPosition());
		borderBack.setPosition(borderGround.getPosition() - glm::vec3(1.0f, 0.0f, 1.0f));
		borderFront.setPosition(borderGround.getPosition() - glm::vec3(1.0f, 0.0f, -sizeGround.depth));
		borderLeft.setPosition(borderGround.getPosition() - glm::vec3(1.0f, 0.0f, 0.0f));
		borderRight.setPosition(borderGround.getPosition() + glm::vec3(sizeGround.width, 0.0f, 0.0f));

		borderGround.rotate(borderAnchor.getRotation());
		borderBack.rotate(borderGround.getRotation());
		borderFront.rotate(borderGround.getRotation());
		borderLeft.rotate(borderGround.getRotation());
		borderRight.rotate(borderGround.getRotation());
		

		Point mousePos = window.getCursorPosition();
		//printf("%f,%f \n", mousePos.position.x, mousePos.position.y);
		

		text_description.scale(glm::vec3(1.0f, 1.0f, 0.01f));
		text_description.scaleToHeight(1.0f);
		text_description.rotate(text_description.getRotation() + glm::vec3(0.0f, 0.0f, 0.0f));

		glm::vec3 camPos = mainCamera.getPosition();
		std::pair<Point, Point> mouseRay = getCameraMousePosition(window, mainCamera);
		btVector3 rayStart = toBtVec3(camPos);
		btVector3 rayEnd = toBtVec3(camPos + mouseRay.second.position * 2.0f * mainCamera.FarZ);
		btCollisionWorld::ClosestRayResultCallback RayCallback(rayStart, rayEnd);
		scene_main.getPhysicsWorld()->rayTest(rayStart, rayEnd, RayCallback);
		if(RayCallback.hasHit()) {
			RigidBody *rBody = nullptr;
			for (auto body : rigidBodys) {
				if (body->getBody() == RayCallback.m_collisionObject) {
					rBody = body;
					break;
				}
			}
			rBody->getBody()->applyCentralImpulse(btVector3(0.0f, 500.0f, 0.0f));

			rayEnd = RayCallback.m_hitPointWorld;
			btVector3 normal = RayCallback.m_hitNormalWorld;
			pLight2.setPosition(toVec3(rayEnd) + glm::vec3(0.0f, 1.0f, 0.0f));
		}
		
		pgTest.setPosition(carChassis.getPositionCenter() - testVehicle->getFront() * 4.0f);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		
		//pLight2.intensity = (sin(glfwGetTime()) * 0.5 + 0.5);    
		//scene_main.draw(deltaTime);
		
		//update vehicle lights
		{
			btTransform transform;
			transform = vehicle->getChassisWorldTransform();
			glm::vec3 carPosition = toVec3(transform.getOrigin());

			frontLight.setPosition(carChassis.getPositionCenter() + glm::vec3(0.0f, 2.0f, 0.0f));
			frontLight.setDirection(testVehicle->getFront() + glm::vec3(0.0f, -0.05f, 0.0f));

			pLightLeft.setPosition(carChassis.getPositionCenter() + glm::vec3(-carChassis.getSize().width * 0.5f + 0.25f, carChassis.getSize().height * 0.5f, -carChassis.getSize().depth * 0.3f));
			pLightLeft.rotate(carChassis.getRotation());

			pLightRight.setPosition(carChassis.getPositionCenter() + glm::vec3(carChassis.getSize().width * 0.5f - 0.75f, carChassis.getSize().height * 0.5f, -carChassis.getSize().depth * 0.3f));
			pLightRight.rotate(carChassis.getRotation());

			pLightLeft.intensity = (sin(lastTime * 0.001f * 18) * 0.5 + 0.5 < 0.5 ? 0.0f : 1.0f);
			pLightRight.intensity = (sin(lastTime * 0.001f * 19 + glm::pi<GLfloat>()) * 0.5 + 0.5 < 0.5 ? 0.0f : 1.0f);
		
			carCamera.setPosition(carPosition + glm::vec3(0.0f, 1.5f, 0.0f));
			glm::vec3 carFront = testVehicle->getFront() * glm::vec3(1.0f, 1.0f, 1.0f);
			carCamera.lookAt(carCamera.getPosition() + carFront);
			mainCamera.lookAt(carPosition);

			carDistanceLine.setPosition(carPosition);
			carDistanceLine.rotate(carChassis.getRotation());


			text_fps.setPosition(carChassis.getPositionCenter() + glm::vec3(0.0f, 2.0f, 0.0f));
			text_fps.rotate(carChassis.getRotation());
			//text_fps.scaleToWidth(test_rect.getSize().width);
			//text_fps.setText(std::to_string((int)abs(vehicle->getCurrentSpeedKmHour())) + " km/h");
			text_fps.setText(std::to_string((int)(1.0f / deltaTime)));


			{
				std::vector<Point> points = carDistanceLine.getPoints();
				btVector3 rayStartc = toBtVec3(carDistanceLine.getModelMatrix() * glm::vec4(points[0].position,1.0f));
				btVector3 rayEnd = toBtVec3(carDistanceLine.getModelMatrix() * glm::vec4(points[1].position, 1.0f));
				btCollisionWorld::ClosestRayResultCallback RayCallback(rayStart, rayEnd);
				scene_main.getPhysicsWorld()->rayTest(rayStart, rayEnd, RayCallback);
				if (false && RayCallback.hasHit()) {
					rayEnd = RayCallback.m_hitPointWorld;
					btVector3 normal = RayCallback.m_hitNormalWorld;
					//std::cout << rayEnd.getY() << std::endl;
					pLight2.setPosition(glm::vec3(rayEnd.getX(), rayEnd.getY(), rayEnd.getZ()));
					std::cout << "to close!!" << std::endl;
					
					vehicle->setBrake(500, 0);
					vehicle->setBrake(500, 1);
					vehicle->setBrake(500, 2);
					vehicle->setBrake(500, 3);

					emergencyBrake = true;
					carDistanceLine.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
				}
				else {
					emergencyBrake = false;
					carDistanceLine.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
				}
			}
			carTrace->color = glm::vec4(glm::min(abs(vehicle->getCurrentSpeedKmHour() / 300.0f), 1.0f), 0.0f, 0.0f, 1.0f);
			carTrace->lineTo(carPosition);
			carSelectedCircle.rotate(glm::vec3(glm::radians(90.0f), carChassis.getRotation().y, carChassis.getRotation().z));
			carSelectedCircle.scale(glm::vec3(1.0f, glm::min(abs(vehicle->getCurrentSpeedKmHour() / 300.0f), 1.0f) * 2.0f + 1.0f, 1.0f));
			carSelectedCircle.setPositionCenter(glm::vec3(carPosition.x, 0.1f, carPosition.z));
			//testCamera.setPosition(carChassis.getPositionCenter() - testVehicle->getFront() + glm::vec3(0.0f, 1.0f, 0.0f));
			//testCamera.lookAt(carPosition + testVehicle->getFront() + glm::vec3(0.0f, 1.0f, 0.0f));

			testCamera.setPosition(carChassis.getPositionCenter() + glm::vec3(0.0f, 60.0f, 0.0f));
		}
		

		scene_main.draw(deltaTime);
		

		window.getWindow()->display();
		GLfloat currentTime = clock.getElapsedTime().asMilliseconds();
		deltaTime = 0.001f * (currentTime - lastTime);
		lastTime = currentTime;
	}


	for (auto body : rigidBodys) {
		while (body->getBody()->getNumConstraintRefs()) {
			btTypedConstraint *constraint = body->getBody()->getConstraintRef(0);
			scene_main.getPhysicsWorld()->removeConstraint(constraint);
		}
	}

	scene_main.dispose();

	dLight.dispose();

	//free memory
	for (auto rbody : rigidBodys) {
		rbody->dispose();
		delete rbody;
	}
	rigidBodys.clear();

	borderGroundShape.dispose();
	borderFrontShape.dispose();
	borderBackShape.dispose();
	borderLeftShape.dispose();
	borderRightShape.dispose();

	borderGround.dispose();
	borderFront.dispose();
	borderBack.dispose();
	borderLeft.dispose();
	borderRight.dispose();

	for(auto cube : cubes) {
		cube->dispose();
		delete cube;
	}

	carChassisShape.dispose();


	//carAnchor.dispose();
	carChassis.dispose();
	for (auto wheel : carWheels) {
		wheel->dispose();
	}
	carWheels.clear();
	delete vehicle;

	terrainShape.dispose();

	instancer.dispose();

	return 0;
}


void handle_key(Window &window) {

	float mouseSpeed = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		mouseSpeed = 150.0f;
	}
	else {
		mouseSpeed = 30.0f;
	}

	glm::vec3 camPos = mainCamera.getPosition();
	glm::vec3 camFront = mainCamera.getFront();
	glm::vec3 camRight = mainCamera.getRight();
	glm::vec3 camUp = mainCamera.getUp();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		mainCamera.setPosition(camPos += camFront * mouseSpeed * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		mainCamera.setPosition(camPos -= camFront * mouseSpeed * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		mainCamera.setPosition(camPos -= camRight * mouseSpeed * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		mainCamera.setPosition(camPos += camRight * mouseSpeed * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		mainCamera.setPosition(camPos -= camUp * mouseSpeed * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		mainCamera.setPosition(camPos += camUp * mouseSpeed * deltaTime);

	sf::Event e;
	while (window.getWindow()->pollEvent(e))
	{
		if (e.type == sf::Event::Closed) {
			window.getWindow()->close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.getWindow()->close();
		}


		vehicle->setBrake(0, 0);
		vehicle->setBrake(0, 1);
		vehicle->setBrake(0, 2);
		vehicle->setBrake(0, 3);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !emergencyBrake) {
			vehicle->applyEngineForce(500, 0);
			vehicle->applyEngineForce(500, 1);
			vehicle->applyEngineForce(10000, 2);
			vehicle->applyEngineForce(10000, 3);
		}
		else {
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				vehicle->applyEngineForce(0, 0);
				vehicle->applyEngineForce(0, 1);
				vehicle->applyEngineForce(0, 2);
				vehicle->applyEngineForce(0, 3);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			vehicle->applyEngineForce(-5000, 0);
			vehicle->applyEngineForce(-5000, 1);
			vehicle->applyEngineForce(-500, 2);
			vehicle->applyEngineForce(-500, 3);
		}
		vehicle->setSteeringValue(btScalar(0.0), 2);
		vehicle->setSteeringValue(btScalar(0.0), 3);
		
		float steering = (1.0f - std::fminf(fabs(testVehicle->getVehicle()->getCurrentSpeedKmHour()) / 150.0f, 0.93f)) * 1.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			vehicle->setSteeringValue(steering, 2);
			vehicle->setSteeringValue(steering, 3);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			vehicle->setSteeringValue(-steering, 2);
			vehicle->setSteeringValue(-steering, 3);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			vehicle->applyEngineForce(btScalar(0), 0);
			vehicle->applyEngineForce(btScalar(0), 1);
			vehicle->applyEngineForce(btScalar(0), 2);
			vehicle->applyEngineForce(btScalar(0), 3);
			vehicle->setBrake(btScalar(400), 0);
			vehicle->setBrake(btScalar(400), 1);
			vehicle->setBrake(btScalar(400), 2);
			vehicle->setBrake(btScalar(400), 3);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
			RigidBody *chassis = testVehicle->getChassis();
			chassis->getBody()->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
			chassis->getBody()->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
			Drawable *dchassis = chassis->getDrawable();
			dchassis->setPositionCenter(glm::vec3(0.0f, 2.0f, 0.0f));
			dchassis->rotate(0.0f, 0.0f, 0.0f);
			chassis->syncBody();
			carTrace->clear();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
			mainScenePtr->renderMode = RenderMode::LINER;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
			mainScenePtr->renderMode = RenderMode::FILLR;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
			sceneCamera = 0;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
			sceneCamera = 1;
		}
	}
}


/*
void key_callback(sf::Window * window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS) {
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}

	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(sf::Window * window, double xpos, double ypos)
{
}

void mouse_scroll_callback(sf::Window * window, double xoffset, double yoffset)
{
}

void handle_key()
{
	float mouseSpeed = 0.0f;
	if (keys[GLFW_KEY_LEFT_SHIFT]) {
		mouseSpeed = 150.0f;
	}
	else {
		mouseSpeed = 30.0f;
	}

	glm::vec3 camPos = mainCamera.getPosition();
	glm::vec3 camFront = mainCamera.getFront();
	glm::vec3 camRight = mainCamera.getRight();
	glm::vec3 camUp = mainCamera.getUp();
	if (keys[GLFW_KEY_W])
		mainCamera.setPosition(camPos += camFront * mouseSpeed * deltaTime);
	if (keys[GLFW_KEY_S])
		mainCamera.setPosition(camPos -= camFront * mouseSpeed * deltaTime);
	if (keys[GLFW_KEY_A])
		mainCamera.setPosition(camPos -= camRight * mouseSpeed * deltaTime);
	if (keys[GLFW_KEY_D])
		mainCamera.setPosition(camPos += camRight * mouseSpeed * deltaTime);
	if (keys[GLFW_KEY_E])
		mainCamera.setPosition(camPos -= camUp * mouseSpeed * deltaTime);
	if (keys[GLFW_KEY_Q])
		mainCamera.setPosition(camPos += camUp * mouseSpeed * deltaTime);
	else if(!emergencyBrake) {
		vehicle->setBrake(0, 0);
		vehicle->setBrake(0, 1);
		vehicle->setBrake(0, 2);
		vehicle->setBrake(0, 3);
	}
	if (keys[GLFW_KEY_UP] && !emergencyBrake) {
		vehicle->applyEngineForce(500, 0);
		vehicle->applyEngineForce(500, 1);
		vehicle->applyEngineForce(10000, 2);
		vehicle->applyEngineForce(10000, 3);
	}
	else {
		if (!keys[GLFW_KEY_DOWN]) {
			vehicle->applyEngineForce(0, 0);
			vehicle->applyEngineForce(0, 1);
			vehicle->applyEngineForce(0, 2);
			vehicle->applyEngineForce(0, 3);
		}
	}
	if (keys[GLFW_KEY_DOWN]) {
		vehicle->applyEngineForce(-5000, 0);
		vehicle->applyEngineForce(-5000, 1);
		vehicle->applyEngineForce(-500, 2);
		vehicle->applyEngineForce(-500, 3);
	}
	vehicle->setSteeringValue(btScalar(0.0), 2);
	vehicle->setSteeringValue(btScalar(0.0), 3);
	float steering = (1.0f - std::fminf(std::fabsf(testVehicle->getVehicle()->getCurrentSpeedKmHour()) / 150.0f, 0.93f)) * 1.0f;
	if (keys[GLFW_KEY_LEFT]) {
		vehicle->setSteeringValue(steering, 2);
		vehicle->setSteeringValue(steering, 3);
	}
	if (keys[GLFW_KEY_RIGHT]) {
		vehicle->setSteeringValue(-steering, 2);
		vehicle->setSteeringValue(-steering, 3);
	}
	if (keys[GLFW_KEY_SPACE]) {
		vehicle->applyEngineForce(0, 0);
		vehicle->applyEngineForce(0, 1);
		vehicle->applyEngineForce(0, 2);
		vehicle->applyEngineForce(0, 3);
		vehicle->setBrake(400, 0);
		vehicle->setBrake(400, 1);
		vehicle->setBrake(400, 2);
		vehicle->setBrake(400, 3);
	}
	if (keys[GLFW_KEY_F]) {
		RigidBody *chassis = testVehicle->getChassis();
		chassis->getBody()->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		chassis->getBody()->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
		Drawable *dchassis = chassis->getDrawable();
		dchassis->setPositionCenter(glm::vec3(0.0f, 2.0f, 0.0f));
		dchassis->rotate(0.0f, 0.0f, 0.0f);
		chassis->syncBody();
		carTrace->clear();
	}

	if (keys[GLFW_KEY_M]) {
		if (mainScenePtr->renderMode == RenderMode::FILLR) {
			mainScenePtr->renderMode = RenderMode::LINER;
		}
		else {
			mainScenePtr->renderMode = RenderMode::FILLR;
		}
	}

}
*/
