#include "subscene.h"

SubScene::SubScene(AbstractRenderer & renderer, Size frame, int sceneWidth, int sceneHeight)
{
	width = sceneWidth;
	height = sceneHeight;

	this->frame = frame;
	this->renderer = &renderer;
	this->camera = renderer.getCamera();
	if(!camera) {
		Log::write(LogType::Error, "Renderer has no camera", "SubScene");
	}
	setup(sceneWidth, sceneHeight);
}

Mesh SubScene::getDebugViewFrustum(int splits) {
	Mesh geoCam;

	ViewFrustum viewF = camera->getViewFrustum(splits);

	geoCam.settings.customColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	geoCam.line(viewF.position, viewF.position + viewF.up);
	geoCam.line(viewF.position, viewF.position + viewF.front);
	geoCam.line(viewF.position, viewF.position + viewF.right);


	geoCam.settings.drawType = DrawType::LINEG;


	for (size_t i = 0; i < viewF.splits.size(); i++) {
		std::vector<glm::vec3> corners = viewF.splits[i];

		geoCam.line(corners[0], corners[1]);
		geoCam.line(corners[1], corners[3]);
		geoCam.line(corners[3], corners[2]);
		geoCam.line(corners[2], corners[0]);


		if (i == viewF.splits.size() - 1) {
			std::vector<glm::vec3> nearCorners = viewF.splits[0];
			geoCam.line(nearCorners[1], corners[1]);
			geoCam.line(nearCorners[3], corners[3]);
			geoCam.line(nearCorners[2], corners[2]);
			geoCam.line(nearCorners[0], corners[0]);
		}
	}
	return geoCam;
}

void SubScene::clear()
{
	renderer->clearScreen();
}

GLuint SubScene::getTexture()
{
	return renderer->getTexture();
}

void SubScene::resizeFrame(Size frame)
{
	this->frame = frame;
	renderer->resize(frame.width * width, frame.height * height);
	renderer->setViewport(Viewport{0, 0, renderer->getWidth(), renderer->getHeight()});

	Size screenSize(
		frame.x * 2 - 1,
		frame.y * 2 - 1,
		0,
		frame.width * 2,
		frame.height * 2,
		0
	);

	//set opengl rectangle size
	GLfloat vertices_rect[] = {
		screenSize.x,                    screenSize.y + screenSize.height, 0.0f, 0.0f, 1.0f, 
		screenSize.x,                    screenSize.y,                 	   0.0f, 0.0f, 0.0f,
		screenSize.x + screenSize.width, screenSize.y + screenSize.height, 0.0f, 1.0f, 1.0f,	
		screenSize.x + screenSize.width, screenSize.y,                 	   0.0f, 1.0f, 0.0f,
	};

	glBindVertexArray(camVAO);
	glBindBuffer(GL_ARRAY_BUFFER, camVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_rect), &vertices_rect, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

Size SubScene::getFrame()
{
	return frame;
}

void SubScene::drawFrame(GLuint target) {
	glBindTexture(GL_TEXTURE_2D, getTexture());

	glBindVertexArray(camVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void SubScene::dispose()
{
	renderer->dispose();

	glDeleteBuffers(1, &camVAO);
	glDeleteBuffers(1, &camVBO);
}

void SubScene::resize(int sceneWidth, int sceneHeight)
{
	width = sceneWidth;
	height = sceneHeight;
	resizeFrame(frame);
}

void SubScene::setup(int sceneWidth, int sceneHeight) {
	glGenVertexArrays(1, &camVAO);
	glGenBuffers(1, &camVBO);
	resize(sceneWidth, sceneHeight);
}