#include "flat.h"

FlatRenderer::FlatRenderer(int width, int height, Camera &camera) : AbstractRenderer(width, height, camera) {
	setup();
	resize(width, height);
}

void FlatRenderer::resize(int width, int height) {
	AbstractRenderer::resize(width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, rendererFBO);
	glBindTexture(GL_TEXTURE_2D, rendererTexture);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, getWidth(), getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rendererTexture, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, rendererRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, getWidth(), getHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rendererRBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Log::write(LogType::Error, "Framebuffer not complete", "FlatRenderer");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if(msaa) {
		glBindFramebuffer(GL_FRAMEBUFFER, multisampleRendererFBO);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampleRendererTexture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RGBA, getWidth(), getHeight(), GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multisampleRendererTexture, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, multisampleRendererRBO);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_DEPTH24_STENCIL8, getWidth(), getHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, multisampleRendererRBO);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Log::write(LogType::Error, "MSAA Framebuffer not complete", "FlatRenderer");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void FlatRenderer::clearScreen() {
	if (msaa == 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, rendererFBO);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, multisampleRendererFBO);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
}

void FlatRenderer::setBackground(unsigned int texture)
{
	if (msaa == 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, rendererFBO);
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, multisampleRendererFBO);
	}
	shaderTexture.use();

	glUniform1i(glGetUniformLocation(shaderTexture.getId(), "screenTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(screenRectVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void FlatRenderer::render() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shaderMesh.use();

	if (msaa == 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, rendererFBO);
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, multisampleRendererFBO);
	}
	glViewport(0, 0, getWidth(), getHeight());
	
	for (auto drawable : drawables) {
		drawable->setCameraMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
		renderDrawableRaw(drawable, shaderMesh, drawable->settings.drawType);
	}

	if (msaa > 0) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleRendererFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rendererFBO);
		glBlitFramebuffer(0, 0, getWidth(), getHeight(), 0, 0, getWidth(), getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FlatRenderer::addSceneObject(SceneObject &sceneObject) {
	if (Drawable *drawable = dynamic_cast<Drawable *>(&sceneObject))
	{
		drawables.push_back(drawable);
		return;
	}

	Log::write(LogType::Error, "Renderer doens't accept " + std::string(sceneObject.getId().c_str()), "FlatRenderer");
}

void FlatRenderer::removeSceneObject(SceneObject &sceneObject) {
	std::remove_if(drawables.begin(), drawables.end(), [&sceneObject](Drawable * drawable) {return &sceneObject == drawable; });
}

GLuint FlatRenderer::getTexture() {
	return rendererTexture;
}

void FlatRenderer::dispose() {
	glDeleteFramebuffers(1, &rendererFBO);
	glDeleteRenderbuffers(1, &rendererRBO);
	glDeleteTextures(1, &rendererTexture);

	glDeleteFramebuffers(1, &multisampleRendererFBO);
	glDeleteRenderbuffers(1, &multisampleRendererRBO);
	glDeleteTextures(1, &multisampleRendererTexture);

	glDeleteBuffers(1, &screenRectVBO);
	glDeleteVertexArrays(1, &screenRectVAO);
}

void FlatRenderer::drawTexture(unsigned int framebuffer)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, rendererFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
	glBlitFramebuffer(0, 0, getWidth(), getHeight(), 0, 0, getWidth(), getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void FlatRenderer::setup() {
	glGenFramebuffers(1, &rendererFBO);
	glGenRenderbuffers(1, &rendererRBO);
	glGenTextures(1, &rendererTexture);

	glGenFramebuffers(1, &multisampleRendererFBO);
	glGenRenderbuffers(1, &multisampleRendererRBO);
	glGenTextures(1, &multisampleRendererTexture);

	GLfloat vertices_rect[] = {
	-1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	 1.0f,	1.0f,  0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
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

	shaderMesh = ResourceManager::loadShader(ShaderName::Renderer::Flat::Mesh);
	shaderTexture = ResourceManager::loadShader(ShaderName::Renderer::Deferred::Pipeline::Texture::ScreenTexture);
}

void FlatRenderer::renderDrawableRaw(Drawable * drawable, Shader shader, DrawType drawType) {

	if (drawable) {
		//set mvp matrix and texture location
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(drawable->getMVPMatrix()));

		glUniform1i(glGetUniformLocation(shader.getId(), "alphaTexture"), 0);
		glUniform1i(glGetUniformLocation(shader.getId(), "diffuseTexture"), 1);
		glUniform1i(glGetUniformLocation(shader.getId(), "specularTexture"), 2);

		//if object is a text object switch to multi texture drawinfg
		//and reduce texture use to alpha texture only
		if (Text * textObject = dynamic_cast<Text*>(drawable)) {
			glUniform1i(glGetUniformLocation(shader.getId(), "enableAlphaTexture"), 1);
			glUniform1i(glGetUniformLocation(shader.getId(), "enableDiffuseTexture"), 0);
			glUniform1i(glGetUniformLocation(shader.getId(), "enableSpecularTexture"), 0);
			for (int i = 0; i < textObject->getText().size(); i++) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textObject->activateChar(i));
				textObject->draw(drawType);
			}
		}
		else {
			//bind textures to shader
			bool useAlphaTexture    = drawable->settings.useAlphaTexture   && drawable->getAlphaTextures().size() > 0;
			bool useSpecularTexture = drawable->settings.useSpecualTexture && drawable->getSpecularTextures().size() > 0;
			bool useDiffuseTexture  = drawable->settings.useDiffuseTexture && drawable->getDiffuseTextures().size() > 0;
			glUniform1i(glGetUniformLocation(shader.getId(), "enableAlphaTexture"),    useAlphaTexture);
			glUniform1i(glGetUniformLocation(shader.getId(), "enableDiffuseTexture"),  useDiffuseTexture);
			glUniform1i(glGetUniformLocation(shader.getId(), "enableSpecularTexture"), useSpecularTexture);

			if(useAlphaTexture) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, drawable->getAlphaTextures()[0]->getGLTexture());
			}
			if(useDiffuseTexture) {
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, drawable->getDiffuseTextures()[0]->getGLTexture());
			}
			if(useSpecularTexture) {
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, drawable->getSpecularTextures()[0]->getGLTexture());
			}

			drawable->draw(drawType);
		}

		for (auto child : drawable->getChildren()) {
			renderDrawableRaw(dynamic_cast<Drawable*>(child), shader, drawType);
		}
	}
	//GLcheckError();
}

