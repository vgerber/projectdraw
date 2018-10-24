#include "uiscene.h"

UiScene::UiScene(float width, float height)
{
	setup();
	reload(width, height);
}

void UiScene::draw(GLfloat delta)
{
}

void UiScene::reload(int width, int height)
{
}

void UiScene::setup()
{
	//renderMode = RenderMode::FILLR;
	
	glGenFramebuffers(1, &gBufferFBO);
	glGenFramebuffers(1, &bloomFBO);
	glGenFramebuffers(1, &lightFBO);
	
	glGenTextures(1, &gBufferPosition);
	glGenTextures(1, &gBufferNormal);
	glGenTextures(1, &gBufferAlbedo);
	glGenTextures(1, &gBufferOption1);
	glGenTextures(1, &gBloom);
	glGenTextures(1, &bloomTexture);
	glGenTextures(1, &lightTexture);

	glGenRenderbuffers(1, &rboGDepth);
	
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, getSceneId(), uboMatrices, 0, 2 * sizeof(glm::mat4) + sizeof(GLfloat));
	
	/*
	GLfloat vertices_rect[] = {
		-1.0f,	1.0f,	0.0f,	0.0f,	1.0f,
		-1.0f,	-1.0f,	0.0f,	0.0f,	0.0f,
		1.0f,	1.0f,	0.0f,	1.0f,	1.0f,
		1.0f,	-1.0f,	0.0f,	1.0f,	0.0f,
	};


	glGenVertexArrays(1, &screenRectVAO);
	glGenBuffers(1, &screenRectVBO);

	glBindVertexArray(screenRectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenRectVBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_rect), &vertices_rect, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
	*/

	/*
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10.0, 0));

	dynamicsWorld->setDebugDrawer(&bulletDebugDrawer);
	bulletDebugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	
	shader_basic = Shaders[SHADER_BASIC];
	shader_light = Shaders[SHADER_DEFFERED_LIGHT];
	shader_normals = Shaders[SHADER_DEFFERED_NORMALS];
	shader_geometry = Shaders[SHADER_DEFFERED_GEOMETRY];
	*/
}
