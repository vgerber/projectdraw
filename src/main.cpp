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
#include "Core/Physics/physics.h"

#ifdef _WIN32
std::string path_obj_mountain = "C:/Users/Vincent/Documents/Projects/Blender/TriFace/basic_mountain.obj";
#endif // _WIN32
#ifdef linux
std::string path_obj_mountain = "/home/vincent/Development/Cpp/opengl/basic_mountain.obj";
#endif // linux


GLfloat deltaTime = 0.0f, mouseX = 0, mouseY = 0, mousePitch = 0, mouseYaw = 0, mouseRoll = 0;
GLfloat WIDTH = 800.0f, HEIGHT = 600.0f;
Camera mainCamera = Camera();

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
	skybox_faces.push_back(Loader::GetPath("/Textures/bubble.png"));
	skybox_faces.push_back(Loader::GetPath("/Textures/bubble.png"));
	skybox_faces.push_back(Loader::GetPath("/Textures/bubble.png"));
	skybox_faces.push_back(Loader::GetPath("/Textures/bubble.png"));
	skybox_faces.push_back(Loader::GetPath("/Textures/bubble.png"));
	skybox_faces.push_back(Loader::GetPath("/Textures/bubble.png"));
	GLuint cubemapTexture = Loader::LoadCubemap(skybox_faces);

	//create camera
	mainCamera.set_position(glm::vec3(0.0f, 5.0f, 10.0f));

	

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
	pLight.set_position(glm::vec3(4.0f, 1.0f, 0.0f));
	pLight.ambient = glm::vec3(0.001f, 0.01f, 0.001f);
	pLight.diffuse = light_color;
	pLight.specular = light_spec;
	pLight.linear = 0.7f;
	pLight.quadratic = 0.007f;

	light_color = glm::vec3(1.0f, 0.2f, 0.0f);
	PointLight pLight2;
	pLight2.set_position(glm::vec3(0.0f, 0.0f, 0.0f));
	pLight2.ambient = glm::vec3(0.001f, 0.01f, 0.001f);
	pLight2.diffuse = light_color;
	pLight2.specular = light_spec;
	pLight2.linear = 0.7f;
	pLight2.quadratic = 0.007f;
	

    Drawable test_obj = Drawable(&path_obj_mountain[0]);
	Drawable ground = Drawable();
	ground.set_model(primitves::generate_quad(30.0f, 1.098f, 30.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));

	Drawable cube = Drawable();
	cube.set_model(primitves::generate_quad(1.0f, 1.0f, 1.0f, glm::vec4(1.0f, 0.3f, 0.8f, 1.0f)));
	

	
	float skyboxSize = 50.0f;
	/*Cube skyboxCube = Cube(2 * skyboxSize, 2 * skyboxSize, 2 * skyboxSize, glm::vec4(0.0f));
	skyboxCube.set_position(glm::vec3(-skyboxSize, -skyboxSize, -skyboxSize));

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
	GLuint hdrFBO;
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


	//
	// TEXT FreeType
	//
	Text text_fps(Loader::GetPath("/Fonts/VeraMono.ttf").c_str(), 60);
	Shaders[SHADER_FONT].use();
	text_fps.scale(0.01f, 0.01f, 0.01f);
	glUniformMatrix4fv(glGetUniformLocation(Shaders[SHADER_FONT].get_id(), "model"), 1, GL_FALSE, glm::value_ptr(text_fps.get_model_matrix()));

	//
	// 
	//


	Scene scene_main;
	scene_main.set_camera(mainCamera);

	scene_main.set_dlight(dLight);
	dLight.change_direction(glm::vec3(-2.0f, -1.0f, 0.0f));
	dLight.draw_shadow = true;

	scene_main.add_plight(pLight);
	scene_main.add_plight(pLight2);
	pLight.set_position(glm::vec3(2.0f, 1.0f, 0.0f));
	pLight2.set_position(glm::vec3(5.0f, 1.0f, 0.0f));

	scene_main.add_object(ground);
	scene_main.add_object(cube);
	scene_main.add_object(test_obj);

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

	test_obj.scale_to_height(1.0f);
	Size size = test_obj.get_size();
	test_obj.set_position(glm::vec3(-5.0, 0.0f, -5.0f));
	test_obj.set_center(glm::vec3(0.5f, 0.5f, 1.7f));
	test_obj.visible_box = true;
	test_obj.visible_normal = false;
	test_obj.draw_type = DrawType::TRIANGLE;

	pLight.set_model(cube.get_model());
	pLight.scale_to_size(size_small);
	pLight2.set_model(cube.get_model());

	//cube.scale_to_size(size_medium);
	cube.set_position(glm::vec3(1.0f, 5.0f, 0.0f));
	cube.rotate(glm::radians(glm::vec3(45.0f, 45.0f, 45.0f)));
	cube.visible_normal = true;


	Drawable test_rect = Drawable();
	test_rect.set_model(primitves::generate_circle(1.0f, 30.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	test_rect.set_position(glm::vec3(-2.0f, 0.0f, -2.0f));
	
	test_rect.draw_type = DrawType::TRIANGLE;
	test_rect.visible_box = true;
	scene_main.add_object(test_rect);


	Geometry testGeometry = Geometry();
	scene_main.add_object(testGeometry);


	scene_main.add_object(text_fps);
	text_fps.visible_box = true;
	text_fps.rotate(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));
	text_fps.set_color(0.1f, 0.1f, 0.1f, 1.0f);
	text_fps.set_position(glm::vec3(-1.0f, 1.0f, -2.0f));


	Text text_description = Text(Loader::GetPath("/Fonts/VeraMono.ttf").c_str(), 60);
	text_description.set_position(glm::vec3(0.0f, 0.1f, -10.0f));
	text_description.rotate(glm::radians(glm::vec3(0.0f, -30.0f, 0.0f)));
	text_description.set_color(1.0f, 0.5f, 0.0f, 1.0f);
	text_description.set_text("This is a great test level!");
	text_description.visible_box = true;
	scene_main.add_object(text_description);


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

	testGeometry.draw_type = DrawType::LINE;

	//cube.set_position(glm::vec3(0.0f, 3.0f, 0.0f));
	

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface *overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld *dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

	btAlignedObjectArray<btCollisionShape*> collisionShapes;


	ground.set_position(glm::vec3(-15.0f, -1.1f, -15.0f));
	ground.rotate(glm::radians(glm::vec3(5.0f, 0.0f, 0.0f)));

	cube.set_position(glm::vec3(0.0f, 10.0f, 0.0f));
	//add ground as ground plane
	{
		collision::CollisionShape ground_shape(collision::generate_cube(ground.get_size()));
		//btCollisionShape *groundShape = //new btBoxShape(btVector3(ground.get_size().width * .5f, ground.get_size().height * .5f, ground.get_size().depth * .5f));//(ground.get_size().width * 0.5f), btScalar(ground.get_size().height * 0.5f), btScalar(ground.get_size().depth * 0.5f)));
		collisionShapes.push_back(ground_shape.get_shape());
		
		btTransform ground_transform;
		ground_transform.setIdentity();
		ground_transform.setOrigin(btVector3(ground.get_position().x + ground.get_size().width * .5f, ground.get_position().y + ground.get_size().height * .5f, ground.get_position().z + ground.get_size().depth * 0.5));//ground.get_position().x, ground.get_position().y, ground.get_position().z));
		ground_transform.setRotation(btQuaternion(ground.get_rotation().y, ground.get_rotation().x, ground.get_rotation().z));
		
		btScalar mass(0.);

		bool isDynamic = (mass != 0.0f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic) {
			ground_shape.get_shape()->calculateLocalInertia(mass, localInertia);
		}

		btDefaultMotionState *myMotionState = new btDefaultMotionState(ground_transform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, ground_shape.get_shape(), localInertia);
		btRigidBody *body = new btRigidBody(rbInfo);
		
		dynamicsWorld->addRigidBody(body);
	}

	//set cube as sceond item
	{
		collision::CollisionShape cube_shape(collision::generate_cube(cube.get_size()));
		collisionShapes.push_back(cube_shape.get_shape());

		btTransform cube_transform;
		cube_transform.setIdentity();
		cube_transform.setOrigin(btVector3(cube.get_position().x + cube.get_size().width * 0.5, cube.get_position().y + cube.get_size().height * .5f, cube.get_position().z + cube.get_size().depth * 0.5));
		cube_transform.setRotation(btQuaternion(cube.get_rotation().y, cube.get_rotation().x, cube.get_rotation().z));
		btScalar mass(0.1f);

		bool isDynamic = (mass != 0.0f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic) {
			cube_shape.get_shape()->calculateLocalInertia(mass, localInertia);
		}

		btDefaultMotionState *myMotionState = new btDefaultMotionState(cube_transform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cube_shape.get_shape(), localInertia);
		btRigidBody *body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(body);
	}

	//simulate
	float bullet_accumulator = 0.0f;


	GLfloat lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		handle_key();
		

		dynamicsWorld->stepSimulation(1.f / 60.f, 100);

		for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--) {
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			
			btTransform trans;
			if (body && body->getMotionState()) {
				body->getMotionState()->getWorldTransform(trans);
			}
			else {
				trans = obj->getWorldTransform();
			}
			
			btScalar rot_x, rot_y, rot_z;
			trans.getRotation().getEulerZYX(rot_z, rot_y, rot_x);

			if (j == 1) {
				cube.set_position(glm::vec3(float(trans.getOrigin().getX() - cube.get_size().width * 0.5), float(trans.getOrigin().getY() - cube.get_size().height * 0.5f), float(trans.getOrigin().getZ() - cube.get_size().depth * 0.5)));
				cube.rotate(glm::vec3(rot_x, rot_y, rot_z));
			}
			if (j == 0) {
				ground.set_position(glm::vec3(float(trans.getOrigin().getX() - ground.get_size().width * 0.5), float(trans.getOrigin().getY()  - ground.get_size().height * 0.5f), float(trans.getOrigin().getZ() - ground.get_size().depth * 0.5)));
				ground.rotate(glm::vec3(rot_x, rot_y, rot_z));
			}


			printf("world pos object %d %f,%f,%f - %f %f %f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()), rot_x, rot_y, rot_z);
		}


		test_rect.rotate(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));

		test_obj.rotate(glm::vec3(0.0f, sin(glfwGetTime()) * glm::radians(30.0f), 0.0f));
		
		//cube.set_position(glm::vec3(0.0f, 1.0f, 0.0f));
		//cube.rotate(cube.get_rotation() + glm::vec3(0.0, 10.0f, 0.0f) * deltaTime);
		text_fps.rotate(text_fps.get_rotation() + glm::vec3(0.0f, 0.0f, 0.0f) * deltaTime);
		
		//testGeometry.line_to(mainCamera.get_position() * glm::vec3(0.0f, 1.0f, 1.0f));

		text_description.scale(glm::vec3(1.0f, 1.0f, 0.01f));
		text_description.scale_to_height(1.0f);
		text_description.rotate(text_description.get_rotation() + glm::vec3(-0.0f, 0.0f, 0.0f));

		
		text_fps.set_position(glm::vec3(-0.99f, 1.0f - text_fps.get_size().height * 0.5f, -2.0f + text_fps.get_size().width * 0.5f));
		text_fps.scale_to_width(test_rect.get_size().width);
		text_fps.set_text(std::to_string(glfwGetTime())); //(int)round(1 / deltaTime)));
		
		pLight.set_position(glm::vec3(-5.0, 1.0f, sin(glfwGetTime()) * 3.0f));

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		dLight.intensity = 1.0f;
		pLight.intensity = 0.4f; // (sin(glfwGetTime() * 15 + 1.7) * 0.5 + 0.5 < 0.5 ? 0.0f : 1.0f);
		pLight2.intensity = (sin(glfwGetTime()) * 0.5 + 0.5);
		scene_main.draw(deltaTime);


		Shaders[SHADER_DEFERRED].use();
		glUniform1f(glGetUniformLocation(Shaders[SHADER_DEFERRED].get_id(), "spotLight.intensity"), 1.0f);
		glUniform3f(glGetUniformLocation(Shaders[SHADER_DEFERRED].get_id(), "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(Shaders[SHADER_DEFERRED].get_id(), "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(Shaders[SHADER_DEFERRED].get_id(), "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(Shaders[SHADER_DEFERRED].get_id(), "spotLight.direction"), 0.0f, -0.1f, -1.0f);
		glUniform3f(glGetUniformLocation(Shaders[SHADER_DEFERRED].get_id(), "spotLight.position"), 5.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(Shaders[SHADER_DEFERRED].get_id(), "spotLight.cutOff"), glm::cos(glm::radians(6.1f)));
		glUniform1f(glGetUniformLocation(Shaders[SHADER_DEFERRED].get_id(), "spotLight.outerCutOff"), glm::cos(glm::radians(7.0f)));

		glDisable(GL_DEPTH_TEST);
		Shaders[SHADER_GEOMETRY].use();
		glBindVertexArray(geoPointVAO);
		glDrawArrays(GL_POINTS, 0, 4);
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);







		glfwSwapBuffers(window);
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
	}
	scene_main.dispose();
	//glDeleteFramebuffers(1, &fbo_texture);


	//free memory
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody *body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}
	for (int i = 0; i < collisionShapes.size(); i++) {
		btCollisionShape* shape = collisionShapes[i];
		collisionShapes[i] = 0;
		delete shape;
	}

	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	collisionShapes.clear();


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
	if (keys[GLFW_KEY_LEFT_SHIFT]) {
		mainCamera.MovementSpeed = 12.0f;
	}
	else {
		mainCamera.MovementSpeed = 3.0f;
	}

}
