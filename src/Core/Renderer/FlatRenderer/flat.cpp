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
	//don't render when width/height is to small
	if(getWidth() <= 0 || getHeight() <= 0) {
		return;
	}
	
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
	
	/*
	for (auto drawable : drawables) {
		drawable->setCameraMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
		renderDrawableRaw(drawable, shaderMesh, drawable->settings.drawType);
	}
	*/
	for(auto sceneObject : sceneObjects) {
		sceneObject->setCamera(*camera);
		sceneObject->draw();
	}

	if (msaa > 0) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleRendererFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rendererFBO);
		glBlitFramebuffer(0, 0, getWidth(), getHeight(), 0, 0, getWidth(), getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FlatRenderer::addSceneObject(SceneObject &sceneObject) {
	sceneObjects.push_back(generateFlatObject(&sceneObject));

	//Log::write(LogType::Error, "Renderer doens't accept " + std::string(sceneObject.getId().c_str()), "FlatRenderer");
}

void FlatRenderer::removeSceneObject(SceneObject &sceneObject) {
	sceneObjects.erase(
		std::remove_if(
			sceneObjects.begin(), 
			sceneObjects.end(), 
			[&sceneObject](FlatSceneObject * child) {return &sceneObject == child->getLinkedObject(); }),
			sceneObjects.end());
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

FlatSceneObject * FlatRenderer::generateFlatObject(SceneObject * sceneObject) {
	FlatSceneObject * newSceneObject = nullptr;

	if(auto mesh = dynamic_cast<Mesh*>(sceneObject)) {
		newSceneObject = new FlatMesh(mesh);
	}
	else if(auto text = dynamic_cast<Text*>(sceneObject)) {
		if(auto mlText = dynamic_cast<MultilineText*>(text)) {
			newSceneObject = new FlatMultilineText(mlText);
		}
		else {
			newSceneObject = new FlatText(text);
		}	
	}
	else if(auto particle = dynamic_cast<PointGenerator2D*>(sceneObject)) {
		if(auto billboard = dynamic_cast<BillboardGenerator2D*>(particle)) {
			newSceneObject = new FlatBillboardGenerator(billboard);
		}
		else {
			newSceneObject = new FlatPointGenerator(particle);
		}
	}
	else {
		newSceneObject = new FlatSceneObject(sceneObject);
	}

	for(auto child : sceneObject->getChildren()) {
		newSceneObject->children.push_back(static_cast<RenderObject*>(generateFlatObject(child)));
	}

	sceneObject->addUpdateTreeListener(this, std::bind(&FlatRenderer::updateSceneObjectTree, this, std::placeholders::_1));
	newSceneObject->update();
	return newSceneObject;	
}

void FlatRenderer::updateSceneObjectTree(SceneObject * sceneObject) {
	FlatSceneObject *  targetSceneObject = nullptr;
	for(auto so : sceneObjects) {
		if(so->getLinkedObject() == sceneObject) {
			targetSceneObject = so;
			break;
		}
	}
	if(targetSceneObject) {
		printf("Target found!\n");
		//remove removed children
		for(int targetChildIndex = 0; targetChildIndex < targetSceneObject->children.size(); targetChildIndex++) {
			RenderObject * targetChild = targetSceneObject->children[targetChildIndex];
			bool childFound = false;
			for(auto refChild : sceneObject->getChildren()) {
				if(refChild == targetChild->getLinkedObject()) {
					childFound = true;
					break;
				}
			}
			if(!childFound) {
				targetSceneObject->children.erase(targetSceneObject->children.begin() + targetChildIndex);
				targetChildIndex--;
			}
		}
		//add added children
		for(auto refChild : sceneObject->getChildren()) {
			bool childFound = false;
			for(auto targetChild : targetSceneObject->children) {
				if(refChild == targetChild->getLinkedObject()) {
					childFound = true;
					break;
				}
			}
			if(!childFound) {
				auto newChild = generateFlatObject(refChild);
				targetSceneObject->children.push_back(static_cast<RenderObject*>(newChild));
			}
		}
	}
}



