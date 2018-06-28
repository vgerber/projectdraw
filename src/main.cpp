#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>

#include <btBulletDynamicsCommon.h>


#include <iostream>

#include "Core/core.h"
#include "Core/Scene/scene.h"
#include "Core/Scene/Camera/camera.h"
#include "Core/Scene/Primitive/primitives.h"
#include "Core/Scene/Light/dlight.h"
#include "Core/Scene/Light/plight.h"
#include "Core/Scene/Text/text.h"

#ifdef _WIN32
std::string path_obj_mountain = "C:/Users/Vincent/Documents/Projects/Blender/TriFace/basic_mountain.obj";
#endif // _WIN32
#ifdef linux
std::string path_obj_mountain = "/home/vincent/Development/Cpp/opengl/basic_mountain.obj";
#endif // linux


GLfloat deltaTime = 0.0f, mouseX = 0, mouseY = 0, mousePitch = 0, mouseYaw = 0, mouseRoll = 0;
GLfloat WIDTH = 800.0f, HEIGHT = 600.0f;
Camera mainCamera = Camera();

btRaycastVehicle *vehicle = nullptr;

bool keys[1024];

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void handle_key();

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Test", nullptr, nullptr);
    
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetKeyCallback(window, key_callback);
    std::cout << glGetString(GL_VERSION) << std::endl;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	init_core();
	//
	// Handle Vertex Objects
	//



	GLfloat vertices_rect[] = {
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	GLfloat points[] = {
		-1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 0.0f, 0.0f
	};
		

	std::vector<std::string> skybox_faces;
	/*skybox_faces.push_back(Loader::GetPath("/Textures/Skybox/right.jpg"));
	skybox_faces.push_back(Loader::GetPath("/Textures/Skybox/left.jpg"));
	skybox_faces.push_back(Loader::GetPath("/Textures/Skybox/top.jpg"));
	skybox_faces.push_back(Loader::GetPath("/Textures/Skybox/bottom.jpg"));
	skybox_faces.push_back(Loader::GetPath("/Textures/Skybox/back.jpg"));
	skybox_faces.push_back(Loader::GetPath("/Textures/Skybox/front.jpg"));
	*/
	/*skybox_faces.push_back(Loader::GetPath("/Textures/bubble.png"));
	skybox_faces.push_back(Loader::GetPath("/Textures/bubble.png"));
	skybox_faces.push_back(Loader::GetPath("/Textures/bubble.png"));
	skybox_faces.push_back(Loader::GetPath("/Textures/bubble.png"));
	skybox_faces.push_back(Loader::GetPath("/Textures/bubble.png"));
	skybox_faces.push_back(Loader::GetPath("/Textures/bubble.png"));
	GLuint cubemapTexture = Loader::LoadCubemap(skybox_faces);
	*/
	//create camera
	mainCamera.setPosition(glm::vec3(0.0f, 5.0f, 10.0f));

	

	//
	// Light
	//
	DirectionalLight dLight = DirectionalLight();
	//dLight.direction = glm::vec3(-2.0f, -2.0f, -2.0f);
	dLight.ambient = glm::vec3(0.001f, 0.001f, 0.001f);
	dLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	dLight.specular = glm::vec3(0.03f, 0.03f, 0.03f);

	glm::vec3 light_color(0.0f, 0.0f, 1.0f);
	glm::vec3 light_spec(0.0f, 0.0f, 1.0f);

	PointLight pLight;
	pLight.setPosition(glm::vec3(4.0f, 1.0f, 0.0f));
	pLight.ambient = glm::vec3(0.001f, 0.01f, 0.001f);
	pLight.diffuse = light_color;
	pLight.specular = light_spec;
	pLight.linear = 0.7f;
	pLight.quadratic = 0.007f;

	light_color = glm::vec3(1.0f, 0.2f, 0.0f);
	PointLight pLight2;
	pLight2.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	pLight2.ambient = glm::vec3(0.001f, 0.01f, 0.001f);
	pLight2.diffuse = light_color;
	pLight2.specular = light_spec;
	pLight2.linear = 0.7f;
	pLight2.quadratic = 0.007f;
	

    Drawable test_obj = Drawable(&path_obj_mountain[0]);


	

	
	float skyboxSize = 50.0f;
	/*Cube skyboxCube = Cube(2 * skyboxSize, 2 * skyboxSize, 2 * skyboxSize, glm::vec4(0.0f));
	skyboxCube.setPosition(glm::vec3(-skyboxSize, -skyboxSize, -skyboxSize));

	GLuint screenRectVBO;
	GLuint screenRectVAO;

	glGenVertexArrays(1, &screenRectVAO);
	glGenBuffers(1, &screenRectVBO);

	glBindVertexArray(screenRectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenRectVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_rect), &vertices_rect, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glBindVertexArray(0);*/

	//
	//Instancing
	//
	glm::vec2 translations[100];
	int index = 0;
	GLfloat offset = 0.1f;
	for (GLint y = -10; y < 10; y += 2) {
		for (GLint x = -10; x < 10; x += 2) {
			glm::vec2 transaltion;
			transaltion.x = (GLfloat)x / 10.0f + offset;
			transaltion.y = (GLfloat)y / 10.0f + offset;
			translations[index++] = transaltion;
		}
	}
	GLuint instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);

	glBindVertexArray(0);

	//
	//Geometry
	//
	
	GLuint geoPointVBO;
	GLuint geoPointVAO;

	glGenVertexArrays(1, &geoPointVAO);
	glGenBuffers(1, &geoPointVBO);

	glBindVertexArray(geoPointVAO);
	glBindBuffer(GL_ARRAY_BUFFER, geoPointVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));
	glBindVertexArray(0);
		




	//
	// HDR
	//
	/*GLuint hdrFBO;
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	// - Create floating point color buffer
	GLuint hdrColorBuffers[2];
	glGenTextures(2, hdrColorBuffers);
	
	for (GLuint i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, hdrColorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, hdrColorBuffers[i], 0);
	}
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// - Create depth buffer (renderbuffer)
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
	// - Attach buffers

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	*/

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


	Scene scene_main;
	scene_main.setCamera(mainCamera);

	scene_main.setDlight(dLight);
	dLight.change_direction(glm::vec3(-2.0f, -1.0f, 0.0f));
	dLight.draw_shadow = true;

	scene_main.addPlight(pLight);
	scene_main.addPlight(pLight2);
	pLight.setPosition(glm::vec3(2.0f, 1.0f, 0.0f));
	pLight2.setPosition(glm::vec3(5.0f, 1.0f, 0.0f));

	

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

	test_obj.scaleToHeight(1.0f);
	Size size = test_obj.getSize();
	test_obj.setPosition(glm::vec3(-5.0, 0.0f, -5.0f));
	test_obj.setCenter(glm::vec3(0.5f, 0.5f, 1.7f));
	test_obj.visibleNormal = false;
	test_obj.drawType = DrawType::TRIANGLE;

	pLight.set_model(primitves::generate_quad(0.5f, 0.5f, 0.5f, glm::vec4(0.8f)));
	pLight2.set_model(pLight.get_model());

	//cube.scaleToSize(size_medium);
	//cube.setPosition(glm::vec3(1.0f, 5.0f, 0.0f));	
	//cube.visibleNormal = true;


	Drawable test_rect = Drawable();
	test_rect.set_model(primitves::generate_circle(1.0f, 30.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	test_rect.setPosition(glm::vec3(0.0f, 5.0f, -2.0f));
	
	test_rect.drawType = DrawType::TRIANGLE;
	scene_main.addDrawable(test_rect);


	Geometry testGeometry = Geometry();
	scene_main.addDrawable(testGeometry);


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
	
	glm::vec3 cubes_position(0.0f, 0.0f, 15.0f);

	for(size_t x = 0; x < 5; x++) {
		for(size_t y = 0;  y < 5; y++) {
			for(size_t z = 0; z < 5; z++) {
				Drawable cube;
				cube.set_model(primitves::generate_quad(1.f, 1.0f, 1.0f, glm::vec4(1.0f, 0.3f, 0.8f, 1.0f)));
				cube.setPosition(cubes_position + glm::vec3(x * 1.0f, y * 1.0f, z * 1.0f));
				cubes.push_back(new Drawable(cube));
				scene_main.addDrawable(*cubes[cubes.size()-1]);
			}
		}
	}


	Drawable borderGround = Drawable();
	borderGround.set_model(primitves::generate_quad(100.0f, 1.0f, 100.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));

	Size sizeGround = borderGround.getSize();
	GLfloat heightScale = 20.0f;

	Drawable borderAnchor, borderBack, borderFront, borderLeft, borderRight;
	borderBack.set_model(primitves::generate_quad(sizeGround.width + 2.0f, sizeGround.height * heightScale, 1.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));
	borderFront.set_model(primitves::generate_quad(sizeGround.width + 2.0f, sizeGround.height * heightScale, 1.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));
	borderLeft.set_model(primitves::generate_quad(1.0f , sizeGround.height * heightScale, sizeGround.depth, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));
	borderRight.set_model(primitves::generate_quad(1.0f, sizeGround.height * heightScale, sizeGround.depth, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));

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
	carChassis.set_model(primitves::generate_quad(4.0f, 1.25f, 7.0f, glm::vec4(0.1f, 0.3f, 0.8f, 1.0f)));
	carChassis.setPositionCenter(carAnchor.getPositionCenter());

	std::vector<Drawable*> carWheels;
	for (int i = 0; i < 4; i++) {
		Drawable *wheel = new Drawable();
		wheel->set_model(primitves::generate_quad(carWheelThickness, 0.5, 0.5, glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)));
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

	scene_main.addDrawable(carChassis);
	for (auto wheel : carWheels) {
		scene_main.addDrawable(*wheel);
	}




	//depth
	glEnable(GL_DEPTH_TEST);	
	glDepthFunc(GL_LESS);

	//glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_CULL_FACE);
	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_FRAMEBUFFER_SRGB);

	
	glEnable(GL_LINE_SMOOTH);
	
	glEnable(GL_LINE_STIPPLE);	
	glLineStipple(1, 0xAAAA);
	glLineWidth(1);
	glPointSize(10);

	testGeometry.drawType = DrawType::LINE;

	//cube.setPosition(glm::vec3(0.0f, 3.0f, 0.0f));
	
	//compensation log db exam?

	std::vector<RigidBody*> rigidBodys;

	borderAnchor.setPositionCenter(glm::vec3(0.0f, 0.0f, 0.0f));

	borderGround.setPositionCenter(borderAnchor.getPositionCenter());
	borderGround.rotate(glm::radians(glm::vec3(0.0f, 0.0f, 0.0f)));
	borderGround.visibleBox = false;

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
	collision::CollisionShape borderGroundShape(collision::generate_cube(borderGround.getSize()));
	collision::CollisionShape borderFrontShape(collision::generate_cube(borderFront.getSize()));
	collision::CollisionShape borderBackShape(collision::generate_cube(borderBack.getSize()));
	collision::CollisionShape borderLeftShape(collision::generate_cube(borderLeft.getSize()));
	collision::CollisionShape borderRightShape(collision::generate_cube(borderRight.getSize()));
	{		
		btCompoundShape* borderCompound = new btCompoundShape();
		btTransform transform;
		transform.setIdentity();

		glm::vec3 posCenter = glm::vec3(0.0f, 0.0f, 0.0f);//borderGround.getPositionCenter();
		transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
		borderCompound->addChildShape(transform, borderGroundShape.get_shape());
		
		posCenter = borderBack.getPositionCenter();
		transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
		borderCompound->addChildShape(transform, borderFrontShape.get_shape());

		posCenter = borderFront.getPositionCenter();
		transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
		borderCompound->addChildShape(transform, borderBackShape.get_shape());
		
		posCenter = borderLeft.getPositionCenter();
		transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
		borderCompound->addChildShape(transform, borderLeftShape.get_shape());

		posCenter = borderRight.getPositionCenter();
		transform.setOrigin(btVector3(posCenter.x, posCenter.y, posCenter.z));
		borderCompound->addChildShape(transform, borderRightShape.get_shape());
		

		collision::CollisionShape borderCompoundShape(borderCompound);

		RigidBody rbody(borderCompoundShape, borderAnchor.getPositionCenter(), borderAnchor.getRotation(), 0.0f);
		rbody.getBody()->setFriction(btScalar(10.0f));
		rbody.setDrawable(borderAnchor);
		rbody.getBody()->setCollisionFlags(rbody.getBody()->getCollisionFlags() | btRigidBody::CF_KINEMATIC_OBJECT);
		rbody.getBody()->setActivationState(DISABLE_DEACTIVATION);
		rbody.visibleAABB = false;
		rigidBodys.push_back(new RigidBody(rbody));

		borderAnchor.setPositionCenter(glm::vec3(0.0f, -sizeGround.height * 0.5f - 3.01f, 0.0f));
		rbody.syncBody();

	}

	//set cube as sceond item
	{
		for(auto cube : cubes) {
			collision::CollisionShape cube_shape(collision::generate_cube(cube->getSize()));
			RigidBody rbodyCube(cube_shape, cube->getPositionCenter(), cube->getRotation(), .1f);
			rbodyCube.setDrawable(*cube);		
			rigidBodys.push_back(new RigidBody(rbodyCube));
		}
	}

	{
		collision::CollisionShape text_shape(collision::generate_sphere(test_rect.getSize().width * 0.5));
		RigidBody rbody(text_shape, test_rect.getPositionCenter(), test_rect.getRotation(), 1.0f);
		rbody.setDrawable(test_rect);
		rigidBodys.push_back(new RigidBody(rbody));
	}

	//set vehicle physics
	collision::CollisionShape carChassisShape = collision::CollisionShape(collision::generate_cube(carChassis.getSize()));
	
	Vehicle *testVehicle;
	{
		btTransform transform;

		RigidBody rbody(carChassisShape, carAnchor.getPositionCenter(), carAnchor.getRotation(), carMass);
		rbody.setDrawable(carAnchor);
		rigidBodys.push_back(new RigidBody(rbody));
		
		testVehicle = new Vehicle(new RigidBody(rbody), scene_main.getPhysicsWorld());

		btScalar connectionHeight(0.0);
		btVector3 wheelConnectionPoint(carChassis.getSize().width * 0.5, connectionHeight, carChassis.getSize().depth * 0.3);
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
		testVehicle->setChassis(&carChassis);

		scene_main.addVehicle(*testVehicle);
		vehicle = testVehicle->getVehicle();


		carAnchor.setPositionCenter(glm::vec3(0.0f, carChassis.getSize().height * 1.0f, 0.0f));
		rbody.syncBody();

	}



	for (auto body : rigidBodys) {
		scene_main.addRigidBody((*body));
	}

	Drawable particle;
	particle.set_model(primitves::generate_quad(1.0f, 1.0f, 1.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));
	ParticleGenerator pgTest(particle, 300);
	scene_main.addParticleGenerator(pgTest);




	GLfloat lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		handle_key();
		
		
		
		//borderGround.setPositionCenter(glm::vec3(0.0f, 0.0f, 0.0f));
		if(glfwGetTime() > 0.0f) { 
			borderAnchor.rotate(glm::radians(glm::vec3(cos((glfwGetTime()) * 1.5f) * 0.0f, glfwGetTime() * 00.0f, cos((glfwGetTime()) * 1.0f) * 0.0f)));
			rigidBodys[0]->syncBody();
		}

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

		
		scene_main.draw(deltaTime);




		test_rect.rotate(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));

		test_obj.rotate(glm::vec3(0.0f, sin(glfwGetTime()) * glm::radians(30.0f), 0.0f));
		
		//cube.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
		//cube.rotate(cube.getRotation() + glm::vec3(0.0, 10.0f, 0.0f) * deltaTime);
		//text_fps.rotate(text_fps.getRotation() + glm::vec3(0.0f, 0.0f, 0.0f) * deltaTime);
		
		//testGeometry.line_to(mainCamera.getPosition() * glm::vec3(0.0f, 1.0f, 1.0f));

		text_description.scale(glm::vec3(1.0f, 1.0f, 0.01f));
		text_description.scaleToHeight(1.0f);
		text_description.rotate(text_description.getRotation() + glm::vec3(0.0f, 0.0f, 0.0f));

		
		text_fps.setPosition(carChassis.getPosition() - glm::vec3(carChassis.getSize().width * 0.5f, 0.0f, -carChassis.getSize().depth * 0.5f)  + glm::vec3(0.0f, 2.0f, 0.0f));
		text_fps.rotate(carChassis.getRotation());
		//text_fps.scaleToWidth(test_rect.getSize().width);
		text_fps.setText(std::to_string((int)abs(vehicle->getCurrentSpeedKmHour())) + " km/h");
		
		pLight.setPosition(glm::vec3(-5.0, 1.0f, sin(glfwGetTime()) * 3.0f));

		btVector3 rayStart(0.0f, 15.0f, 0.0f);
		btVector3 rayEnd(0.0f, -5.0f, 2.0f);
		btCollisionWorld::ClosestRayResultCallback RayCallback(rayStart, rayEnd);
		scene_main.getPhysicsWorld()->rayTest(rayStart, rayEnd, RayCallback);
		if(RayCallback.hasHit()) {
			rayEnd = RayCallback.m_hitPointWorld;
			btVector3 normal = RayCallback.m_hitNormalWorld;
			//std::cout << rayEnd.getY() << std::endl;
			pLight2.setPosition(glm::vec3(rayEnd.getX(), rayEnd.getY(), rayEnd.getZ()));
			//std::cout << pLight2.getPosition().y << std::endl;
		}

		pgTest.setPosition(carChassis.getPositionCenter());


		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		dLight.intensity = 1.0f;
		pLight.intensity = 0.4f; // (sin(glfwGetTime() * 15 + 1.7) * 0.5 + 0.5 < 0.5 ? 0.0f : 1.0f);
		pLight2.intensity = (sin(glfwGetTime()) * 0.5 + 0.5);
		//scene_main.draw(deltaTime);
		
		btTransform transform;
		transform = vehicle->getChassisWorldTransform();
		glm::vec3 carPosition = toVec3(transform.getOrigin());


		Shaders[SHADER_DEFERRED].use();
		glUniform1f(glGetUniformLocation(Shaders[SHADER_DEFERRED].getId(), "spotLight.intensity"), 1.0f);
		glUniform3f(glGetUniformLocation(Shaders[SHADER_DEFERRED].getId(), "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(Shaders[SHADER_DEFERRED].getId(), "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(Shaders[SHADER_DEFERRED].getId(), "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(Shaders[SHADER_DEFERRED].getId(), "spotLight.direction"), testVehicle->getDirection().x, testVehicle->getDirection().y, testVehicle->getDirection().z);
		glUniform3f(glGetUniformLocation(Shaders[SHADER_DEFERRED].getId(), "spotLight.position"), carPosition.x, carPosition.y, carPosition.z);
		glUniform1f(glGetUniformLocation(Shaders[SHADER_DEFERRED].getId(), "spotLight.cutOff"), glm::cos(glm::radians(6.1f)));
		glUniform1f(glGetUniformLocation(Shaders[SHADER_DEFERRED].getId(), "spotLight.outerCutOff"), glm::cos(glm::radians(7.0f)));

		glDisable(GL_DEPTH_TEST);
		Shaders[SHADER_GEOMETRY].use();
		glBindVertexArray(geoPointVAO);
		glDrawArrays(GL_POINTS, 0, 4);
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);




		Size viewFrustum = mainCamera.getViewFrustum();

		std::cout << viewFrustum.width << " " << viewFrustum.height << std::endl;


		glfwSwapBuffers(window);
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
	}


	for (auto body : rigidBodys) {
		while (body->getBody()->getNumConstraintRefs()) {
			btTypedConstraint *constraint = body->getBody()->getConstraintRef(0);
			scene_main.getPhysicsWorld()->removeConstraint(constraint);
		}
	}

	scene_main.dispose();
	//glDeleteFramebuffers(1, &fbo_texture);


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

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
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

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{

	GLfloat xoffset = (xpos - mouseX);
	GLfloat yoffset = (mouseY - ypos);

	mainCamera.HandleMouseMove(xoffset, yoffset);

	mouseX = xpos;
	mouseY = ypos;
}

void mouse_scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	mainCamera.HandleMouseScroll(yoffset);
}

void handle_key()
{
	if (keys[GLFW_KEY_W])
		mainCamera.HandleKeyboard(CameraMovement::FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		mainCamera.HandleKeyboard(CameraMovement::BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		mainCamera.HandleKeyboard(CameraMovement::LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		mainCamera.HandleKeyboard(CameraMovement::RIGHT, deltaTime);
	if (keys[GLFW_KEY_SPACE]) {
		vehicle->applyEngineForce(0, 0);
		vehicle->applyEngineForce(0, 1);
		vehicle->setBrake(60, 0);
		vehicle->setBrake(60, 1);
		vehicle->setBrake(60, 2);
		vehicle->setBrake(60, 3);
	}
	else {
		vehicle->setBrake(0, 0);
		vehicle->setBrake(0, 1);
		vehicle->setBrake(0, 2);
		vehicle->setBrake(0, 3);
	}
	if (keys[GLFW_KEY_UP]) {
		vehicle->applyEngineForce(500, 0);
		vehicle->applyEngineForce(500, 1);
		vehicle->applyEngineForce(3500, 2);
		vehicle->applyEngineForce(3500, 3);
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
		vehicle->applyEngineForce(-3500, 0);
		vehicle->applyEngineForce(-3500, 1);
		vehicle->applyEngineForce(-500, 2);
		vehicle->applyEngineForce(-500, 3);
	}
	vehicle->setSteeringValue(btScalar(0.0), 2);
	vehicle->setSteeringValue(btScalar(0.0), 3);
	if (keys[GLFW_KEY_LEFT]) {
		vehicle->setSteeringValue(btScalar(0.7), 2);
		vehicle->setSteeringValue(btScalar(0.7), 3);
	}
	if (keys[GLFW_KEY_RIGHT]) {
		vehicle->setSteeringValue(btScalar(-0.7), 2);
		vehicle->setSteeringValue(btScalar(-0.7), 3);
	}
	if (keys[GLFW_KEY_R]) {
		
	}


	if (keys[GLFW_KEY_LEFT_SHIFT]) {
		mainCamera.MovementSpeed = 12.0f;
	}
	else {
		mainCamera.MovementSpeed = 3.0f;
	}

}
