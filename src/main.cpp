#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>

#include <iostream>

#include "Core/Scene/Camera/camera.h"
#include "Core/Scene/Primitive/3D/cube.h"
#include "Core/Scene/Light/dlight.h"
#include "Core/Scene/Light/plight.h"
#include "Core/Scene/Text/text.h"

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
	mainCamera.SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));

	

	//
	// Light
	//
	DirectionalLight dLight;
	dLight.direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	dLight.ambient = glm::vec3(0.001f, 0.001f, 0.001f);
	dLight.diffuse = glm::vec3(0.01f, 0.01f, 0.01f);
	dLight.specular = glm::vec3(0.03f, 0.03f, 0.03f);

	PointLight pLight;
	pLight.SetPosition(glm::vec3(4.0f, 2.0f, 0.0f));
	pLight.ambient = glm::vec3(0.001f, 0.01f, 0.001f);
	pLight.diffuse = glm::vec3(0.0, 1.0, 0.0f);
	pLight.specular = glm::vec3(0.0f, 1.0f, 0.0f);
	pLight.linear = 0.07f;
	pLight.quadratic = 0.007f;

	//lampRect.SetPosition(lightPos);
	
	/*Material materialMat;
	materialMat.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	materialMat.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	materialMat.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	materialMat.shininess = 32.0f;*/

	//glUniform1i(shader_materialDiffuseLocation, 0);
	//glUniform1i(shader_materialSpecularLocation, 1);

	/*
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_brick);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_brick_spec_map);
    
    */
	Shader shader_geometry;
	shader_geometry.Load(
		Loader::GetPath("/Shaders/geometry.vertex").c_str(), 
		Loader::GetPath("/Shaders/geometry.fragment").c_str(),
		Loader::GetPath("/Shaders/geometry.geometry").c_str()
	);

	Shader shader_normal_geometry;
	shader_normal_geometry.Load(
		Loader::GetPath("/Shaders/normal.vertex").c_str(),
		Loader::GetPath("/Shaders/normal.fragment").c_str(),
		Loader::GetPath("/Shaders/normal.geometry").c_str()
	);

	Shader shader_depth_cube;
	shader_depth_cube.Load(
		Loader::GetPath("/Shaders/cube_depth.vertex").c_str(),
		Loader::GetPath("/Shaders/cube_depth.fragment").c_str(),
		Loader::GetPath("/Shaders/cube_depth.geometry").c_str()
	);

	Shader shader_instancing;
	shader_instancing.Load(
		Loader::GetPath("/Shaders/instancing.vertex").c_str(),
		Loader::GetPath("/Shaders/instancing.fragment").c_str()
	);

	Shader shader_basic;
	shader_basic.Load(
		Loader::GetPath("/Shaders/basic.vertex").c_str(),
		Loader::GetPath("/Shaders/basic.fragment").c_str()
	);

	Shader shader1; 
	shader1.Load(
		Loader::GetPath("/Shaders/basic.vertex").c_str(), 
		Loader::GetPath("/Shaders/lamp.fragment").c_str()
	);

	Shader shader_deferred;
	shader_deferred.Load(
		Loader::GetPath("/Shaders/texture.vertex").c_str(),
		Loader::GetPath("/Shaders/deferred.fragment").c_str()
	);
    //Rectangle test_rect = Rectangle(-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f);
	

    Drawable test_obj = Drawable("C:/Users/Vincent/Documents/Projects/Blender/TriFace/basic_mountain.obj");
	Cube ground = Cube(-100.0f, -0.1f, -100.0f, 200.0f, 0.098f, 200.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

	Cube cube = Cube(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
	
	Shader shader_font;
	shader_font.Load(
		Loader::GetPath("/Shaders/font.vertex").c_str(),
		Loader::GetPath("/Shaders/font.fragment").c_str()
	);

	Shader shader_screen;
	shader_screen.Load(
		Loader::GetPath("/Shaders/texture.vertex").c_str(),
		Loader::GetPath("/Shaders/texture.fragment").c_str()
	);
	
	Shader shader_depth;
	shader_depth.Load(
		Loader::GetPath("/Shaders/depth.vertex").c_str(),
		Loader::GetPath("/Shaders/depth.fragment").c_str()
	);

	Shader shader_skybox;
	shader_skybox.Load(
		Loader::GetPath("/Shaders/skybox.vertex").c_str(), 
		Loader::GetPath("/Shaders/skybox.fragment").c_str()
	);
	
	float skyboxSize = 50.0f;
	Cube skyboxCube = Cube(-skyboxSize, -skyboxSize, -skyboxSize, 2 * skyboxSize, 2 * skyboxSize, 2 * skyboxSize, glm::vec4(0.0f));

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
	glBindVertexArray(0);

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
	// frame buffers
	//
	/*GLuint fbo_texture;
	glGenFramebuffers(1, &fbo_texture);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_texture);

	GLuint texture_color_buffer;
	glGenTextures(1, &texture_color_buffer);
	glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//unbind buffer
	glBindTexture(GL_TEXTURE_2D, 0);
	//attach color buffer to frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_buffer, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindBuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	//unbind buffer
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//attach buffer to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER Framebuffer is not complete" << std::endl;
	}
	//unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/



	//
	// Uniform Block Binding
	//
	glUniformBlockBinding(shader_basic.GetProgrammId(), glGetUniformBlockIndex(shader_basic.GetProgrammId(), "Matrices"), 0);
	glUniformBlockBinding(shader_basic.GetProgrammId(), glGetUniformBlockIndex(shader_skybox.GetProgrammId(), "Matrices"), 0);
	glUniformBlockBinding(shader_basic.GetProgrammId(), glGetUniformBlockIndex(shader_normal_geometry.GetProgrammId(), "Matrices"), 0);
	glUniformBlockBinding(shader_basic.GetProgrammId(), glGetUniformBlockIndex(shader_normal_geometry.GetProgrammId(), "Matrices"), 0);

	GLuint uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));



	//
	//Shadow Mapping
	//
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat bordercolor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	GLfloat near_plane = 0.1f, far_plane = 100.0f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(-dLight.direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;


	//
	// Shadow mapping PointLight
	//
	GLuint depthCubeMap;
	glGenTextures(1, &depthCubeMap);
	const GLuint SHADOW_C_WIDTH = 1024, SHADOW_C_HEIGHT = 1024;
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	for (GLuint i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_C_WIDTH, SHADOW_C_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	GLuint depthCubemapFBO;
	glGenFramebuffers(1, &depthCubemapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER Framebuffer is not complete" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	GLfloat aspect = (GLfloat)SHADOW_C_WIDTH / (GLfloat)SHADOW_C_HEIGHT;
	GLfloat near = 1.0f;
	GLfloat far = 100.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj *	glm::lookAt(pLight.GetPosition(), pLight.GetPosition() + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *	glm::lookAt(pLight.GetPosition(), pLight.GetPosition() + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *	glm::lookAt(pLight.GetPosition(), pLight.GetPosition() + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj *	glm::lookAt(pLight.GetPosition(), pLight.GetPosition() + glm::vec3(0.0, -1.0, 0.0),	glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj *	glm::lookAt(pLight.GetPosition(), pLight.GetPosition() + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pLight.GetPosition(), pLight.GetPosition() + glm::vec3(0.0, 0.0, -1.0),	glm::vec3(0.0, -1.0, 0.0)));



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
	// Deferred Shading
	//
	GLuint gBufferFBO;
	glGenFramebuffers(1, &gBufferFBO);	
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	GLuint gBufferPosition, gBufferNormal, gBufferAlbedo;
	glGenTextures(1, &gBufferPosition);
	glBindTexture(GL_TEXTURE_2D, gBufferPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gBufferPosition, 0);

	glGenTextures(1, &gBufferNormal);
	glBindTexture(GL_TEXTURE_2D, gBufferNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gBufferNormal, 0);
	
	glGenTextures(1, &gBufferAlbedo);
	glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gBufferAlbedo, 0);

	GLuint gAttachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, gAttachments);

	GLuint rboGDepth;
	glGenRenderbuffers(1, &rboGDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboGDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
	// - Attach buffers

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboGDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	//
	// TEXT FreeType
	//
	Font fontVera(Loader::GetPath("/Fonts/VeraMono.ttf").c_str(), 20);
	glm::mat4 font_proj = glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT);
	glm::mat4 font_model = glm::mat4(1.0f);
	glm::mat4 font_view = glm::mat4(1.0f);
	shader_font.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader_font.GetProgrammId(), "projection"), 1, GL_FALSE, glm::value_ptr(font_proj));
	glUniformMatrix4fv(glGetUniformLocation(shader_font.GetProgrammId(), "view"), 1, GL_FALSE, glm::value_ptr(font_view));
	glUniformMatrix4fv(glGetUniformLocation(shader_font.GetProgrammId(), "model"), 1, GL_FALSE, glm::value_ptr(font_model));


	//depth
	glEnable(GL_DEPTH_TEST);	
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_CULL_FACE);
	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_FRAMEBUFFER_SRGB);

	cube.Translate(0.5f, 0.5f, 0.5f);

	GLfloat lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		handle_key();


		test_obj.Scale(2.0f, 2.0f, 2.0f);
		test_obj.Rotate(test_obj.GetRotion() + glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime);
		test_obj.Translate(sin(glfwGetTime()) * 4.0f, 0.0f, -5.0f);
		cube.Rotate(cube.GetRotion() + glm::vec3(0.0, 1.0f, 0.0f) * deltaTime);
		//
		// Shadow mapping
		//
		glCullFace(GL_FRONT);		
		shader_depth.Use();
		glUniformMatrix4fv(glGetUniformLocation(shader_depth.GetProgrammId(), "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		

		//render Scne in Depth Buffer
		glUniformMatrix4fv(glGetUniformLocation(shader_depth.GetProgrammId(), "model"), 1, GL_FALSE, glm::value_ptr(ground.GetModelMatrix()));
		ground.Draw(shader_basic);
		glUniformMatrix4fv(glGetUniformLocation(shader_depth.GetProgrammId(), "model"), 1, GL_FALSE, glm::value_ptr(cube.GetModelMatrix()));
		cube.Draw(shader_basic);
		glUniformMatrix4fv(glGetUniformLocation(shader_depth.GetProgrammId(), "model"), 1, GL_FALSE, glm::value_ptr(test_obj.GetModelMatrix()));
		test_obj.Draw(shader_basic);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		


		//Shadow Mapping PointLight
		shader_depth_cube.Use();
		glViewport(0, 0, SHADOW_C_WIDTH, SHADOW_C_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		glUniformMatrix4fv(glGetUniformLocation(shader_depth_cube.GetProgrammId(), "shadowMatrices[0]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
		glUniformMatrix4fv(glGetUniformLocation(shader_depth_cube.GetProgrammId(), "shadowMatrices[1]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[1]));
		glUniformMatrix4fv(glGetUniformLocation(shader_depth_cube.GetProgrammId(), "shadowMatrices[2]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[2]));
		glUniformMatrix4fv(glGetUniformLocation(shader_depth_cube.GetProgrammId(), "shadowMatrices[3]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[3]));
		glUniformMatrix4fv(glGetUniformLocation(shader_depth_cube.GetProgrammId(), "shadowMatrices[4]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[4]));
		glUniformMatrix4fv(glGetUniformLocation(shader_depth_cube.GetProgrammId(), "shadowMatrices[5]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[5]));
		glUniform1f(glGetUniformLocation(shader_depth_cube.GetProgrammId(), "far_plane"), far);
		glUniform3f(glGetUniformLocation(shader_depth_cube.GetProgrammId(), "lightPos"), pLight.GetPosition().x, pLight.GetPosition().y, pLight.GetPosition().z);

		//render Scene in Depth Buffer
		glUniformMatrix4fv(glGetUniformLocation(shader_depth_cube.GetProgrammId(), "model"), 1, GL_FALSE, glm::value_ptr(ground.GetModelMatrix()));
		ground.Draw(shader_depth_cube);
		glUniformMatrix4fv(glGetUniformLocation(shader_depth_cube.GetProgrammId(), "model"), 1, GL_FALSE, glm::value_ptr(cube.GetModelMatrix()));
		cube.Draw(shader_depth_cube);
		glUniformMatrix4fv(glGetUniformLocation(shader_depth_cube.GetProgrammId(), "model"), 1, GL_FALSE, glm::value_ptr(test_obj.GetModelMatrix()));
		test_obj.Draw(shader_depth_cube);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glCullFace(GL_BACK);		

		//
		//Draw Scene
		//
		glViewport(0, 0, WIDTH, HEIGHT);

		//HDR
		glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mainCamera.GetViewMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mainCamera.GetCameraMatrix(WIDTH, HEIGHT)));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//glClearColor(0.4f, 0.4f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//glEnable(GL_DEPTH_TEST);

		shader_basic.Use();

        //glUseProgram(shader.GetProgrammId());

		glUniformMatrix4fv(glGetUniformLocation(shader_basic.GetProgrammId(), "model"), 1, GL_FALSE, glm::value_ptr(ground.GetModelMatrix()));
		ground.Draw(shader_basic);
		



		glUniformMatrix4fv(glGetUniformLocation(shader_basic.GetProgrammId(), "model"), 1, GL_FALSE, glm::value_ptr(test_obj.GetModelMatrix()));
		test_obj.Draw(shader_basic);
		
		//draw normals
		/*shader_normal_geometry.Use();
		glUniformMatrix4fv(glGetUniformLocation(shader_normal_geometry.GetProgrammId(), "model"), 1, GL_FALSE, glm::value_ptr(test_obj.GetModelMatrix()));
		test_obj.Draw(shader_normal_geometry);
		shader_basic.Use();*/

		glClear(GL_STENCIL_BUFFER_BIT);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		//test_obj.Draw();
		
		glStencilFunc(GL_ALWAYS, 1, 0xFF);


		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture_window);
		//glUniform1i(glGetUniformLocation(cube.GetShader().GetProgrammId(), "tex"), 0);
		glUniformMatrix4fv(glGetUniformLocation(shader_basic.GetProgrammId(), "model"), 1, GL_FALSE, glm::value_ptr(cube.GetModelMatrix()));
		cube.Draw(shader_basic);

		/*glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);	

		glUseProgram(shader1.GetProgrammId());
		float scale = 0.1f;
		float trans = scale / 2;
		rect.Translate(-trans, -trans, -trans);
		rect.Scale(1.0f + scale, 1.0f + scale, 1.0f + scale);
		glUniformMatrix4fv(glGetUniformLocation(shader1.GetProgrammId(), "view"), 1, GL_FALSE, glm::value_ptr(mainCamera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shader1.GetProgrammId(), "projection"), 1, GL_FALSE, glm::value_ptr(mainCamera.GetCameraMatrix(WIDTH, HEIGHT)));
		glUniformMatrix4fv(glGetUniformLocation(shader1.GetProgrammId(), "model"), 1, GL_FALSE, glm::value_ptr(rect.GetModelMatrix()));
		glUniform3f(glGetUniformLocation(shader1.GetProgrammId(), "lightColor"), 0.5f, 1.0f, 1.0f);
		rect.Draw();		
		rect.Scale(1.0f, 1.0f, 1.0f);
		rect.Translate(trans, trans, trans);
		
		glStencilMask(0xFF);
		
		glEnable(GL_DEPTH_TEST);*/

		
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		
		
		
		//
		// Skybox
		//
		/*/
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);
		shader_skybox.Use();

		glm::mat4 view = glm::mat4(glm::mat3(mainCamera.GetViewMatrix()));				
		
		glUniform1i(glGetUniformLocation(shader_skybox.GetProgrammId(), "skybox"), 0);
		glActiveTexture(GL_TEXTURE0);		
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		skyboxCube.Draw(shader_skybox);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		*/
		//
		// Geometry Corners
		//
		shader_geometry.Use();
		glBindVertexArray(geoPointVAO);
		glDrawArrays(GL_POINTS, 0, 4);
		glBindVertexArray(0);
		

		//
		// Instancing
		//
		/*glClear(GL_DEPTH_BUFFER_BIT);
		shader_instancing.Use();
		glBindVertexArray(screenRectVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
		glBindVertexArray(0);*/

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//
		// texture Scene
		//
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_deferred.Use();
		dLight.Apply(shader_deferred, "dirLight");
		pLight.Apply(shader_deferred, "pointLight");
		glUniform1f(glGetUniformLocation(shader_deferred.GetProgrammId(), "far_plane"), far);
		glUniform3f(glGetUniformLocation(shader_deferred.GetProgrammId(), "viewPos"), mainCamera.GetPosition().x, mainCamera.GetPosition().y, mainCamera.GetPosition().z);
		glUniformMatrix4fv(glGetUniformLocation(shader_deferred.GetProgrammId(), "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

		glUniform1i(glGetUniformLocation(shader_deferred.GetProgrammId(), "shadowMap"),	 0);
		glUniform1i(glGetUniformLocation(shader_deferred.GetProgrammId(), "shadowCubeMap"), 1);
		glUniform1i(glGetUniformLocation(shader_deferred.GetProgrammId(), "gPosition"),  2);
		glUniform1i(glGetUniformLocation(shader_deferred.GetProgrammId(), "gNormal"),	 3);
		glUniform1i(glGetUniformLocation(shader_deferred.GetProgrammId(), "gAlbedo"),	 4);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBufferPosition);		
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gBufferNormal);		
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);

		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);


		glClear(GL_DEPTH_BUFFER_BIT);
		shader_font.Use();
		//fontVera.RenderText(shader_font, std::to_string((int)round(1 / deltaTime)), 1.0f, 580.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));


		glfwSwapBuffers(window);
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
	}

	//glDeleteFramebuffers(1, &fbo_texture);

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
