#include "scenecamera.h"

SceneCamera::SceneCamera(Camera & camera, Size frame)
{
	this->camera = &camera;
	this->frame = frame;
	setup();
}

void SceneCamera::beginDrawing(Shader shader)
{
	if (Width != camera->Width || Height != camera->Height) {
		reload();
		Width = camera->Width;
		Height = camera->Height;
	}


	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.use();
}

void SceneCamera::endDrawing()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint SceneCamera::getTexture()
{
	return sceneTexture;
}

GLuint SceneCamera::getFrameVerticesVAO()
{
	return VAO;
}

void SceneCamera::setFrame(Size frame)
{
	this->frame = frame;
	reload();
}

void SceneCamera::dispose()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &sceneTexture);
	glDeleteFramebuffers(1, &FBO);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

void SceneCamera::setup()
{
	glGenFramebuffers(1, &FBO);
	glGenTextures(1, &sceneTexture);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	reload();
	
}

void SceneCamera::reload()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindTexture(GL_TEXTURE_2D, sceneTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, camera->Width, camera->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTexture, 0);
	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLfloat vertices_rect[] = {
		frame.x,			   frame.y + frame.height,	0.0f,	0.0f,	1.0f,
		frame.x,			   frame.y,					0.0f,	0.0f,	0.0f,
		frame.x + frame.width, frame.y + frame.height,	0.0f,	1.0f,	1.0f,
		frame.x + frame.width, frame.y,					0.0f,	1.0f,	0.0f,
	};


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_rect), &vertices_rect, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
}
