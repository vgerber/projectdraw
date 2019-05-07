#include "forward.h"

ForwardRenderer::ForwardRenderer(int width, int height, Camera &camera) : AbstractRenderer(width, height, camera) {
    setup();
    resize(width, height);
}

void ForwardRenderer::resize(int width, int height) {
	AbstractRenderer::resize(width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, rendererFBO);
    glBindTexture(GL_TEXTURE_2D, rendererTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, getWidth(), getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rendererTexture, 0);
	//unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
    //glDrawBuffers(1, attachments);

    glBindRenderbuffer(GL_RENDERBUFFER, rendererRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, getWidth(), getHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rendererRBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glBindFramebuffer(GL_FRAMEBUFFER, depthInfoFBO);
    glBindTexture(GL_TEXTURE_2D, depthInfoPositionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, getWidth(), getHeight(), 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depthInfoPositionTexture, 0);

	glBindTexture(GL_TEXTURE_2D, depthInfoNormalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, getWidth(), getHeight(), 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, depthInfoNormalTexture, 0);
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    glBindRenderbuffer(GL_RENDERBUFFER, depthInfoRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, getWidth(), getHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthInfoRBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	hdrFilter.resize(getWidth(), getHeight());
	ssaoFilter.resize(getWidth(), getHeight());
}

void ForwardRenderer::clearScreen() {
    glBindFramebuffer(GL_FRAMEBUFFER, rendererFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthInfoFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	ssaoFilter.clear();
}

void ForwardRenderer::render() {
	glViewport(0, 0, getWidth(), getHeight());

	if(!directionalLight) {
		Log::write(LogType::Error, "Directional light not set", "ForwardRenderer");
		return;
	}
	
	GLcheckError();
	glBindFramebuffer(GL_FRAMEBUFFER, depthInfoFBO);
	Shader shaderDepthMesh = ResourceManager::loadShader(ShaderName::Renderer::Forward::DepthInfo::Mesh);
	shaderDepthMesh.use();
	glUniformMatrix4fv(glGetUniformLocation(shaderDepthMesh.getId(), "view"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	for(auto sceneObject : sceneObjects) {
		sceneObject->setCamera(*camera);
		sceneObject->drawDepthInfo();
    }
	ssaoFilter.apply(*camera, depthInfoNormalTexture, depthInfoPositionTexture);

    shaderMesh.use();
	std::string target = "directionalLight";
	glUniform1f(glGetUniformLocation(shaderMesh.getId(), (target + ".intensity").c_str()), directionalLight->intensity);
	glm::vec3 direction = directionalLight->getDirection();
    glm::vec3 ambient = directionalLight->ambient;
    glm::vec3 diffuse = directionalLight->diffuse;
    glm::vec3 specular = directionalLight->specular;
	glUniform3f(glGetUniformLocation(shaderMesh.getId(), (target + ".direction").c_str()), direction.x, direction.y, direction.z);
	glUniform3f(glGetUniformLocation(shaderMesh.getId(), (target + ".ambient").c_str()), ambient.r, ambient.g, ambient.b);
	glUniform3f(glGetUniformLocation(shaderMesh.getId(), (target + ".diffuse").c_str()), diffuse.r, diffuse.g, diffuse.b);
	glUniform3f(glGetUniformLocation(shaderMesh.getId(), (target + ".specular").c_str()), specular.r, specular.g, specular.b);

	glUniform1i(glGetUniformLocation(shaderMesh.getId(), "ssaoTexture"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, ssaoFilter.getGLTexture());

	//GLcheckError();

	glUniformBlockBinding(shaderMesh.getId(), glGetUniformBlockIndex(shaderMesh.getId(), "Matrices"), getRendererId());
	float farZ = camera->getClippingFar();
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera->getViewMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->getProjectionMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(GLfloat), &farZ);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, rendererFBO);

	glm::vec3 viewPos = camera->getWorldTransform().getTranslation();
	glUniform3f(glGetUniformLocation(shaderMesh.getId(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);

    for(auto sceneObject : sceneObjects) {
		sceneObject->setCamera(*camera);
		sceneObject->draw();
    }

	hdrFilter.apply(rendererTexture);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);	
}

void ForwardRenderer::addSceneObject(SceneObject &sceneObject) {

	if(DirectionalLight * dirLight = dynamic_cast<DirectionalLight*>(&sceneObject)) {
		this->directionalLight = dirLight;
		return;
	}

	sceneObjects.push_back(generateForwardObject(&sceneObject));
}

void ForwardRenderer::removeSceneObject(SceneObject &sceneObject) {
	sceneObjects.erase(
		std::remove_if(
			sceneObjects.begin(),
			sceneObjects.end(),
			[&sceneObject](ForwardSceneObject * child) {
				return child->getLinkedObject() == &sceneObject;
			} 
		)
	);
}

GLuint ForwardRenderer::getTexture() {
    return rendererTexture;
}

void ForwardRenderer::dispose() {
    glDeleteFramebuffers(1, &rendererFBO);
    glDeleteRenderbuffers(1, &rendererRBO);
    glDeleteTextures(1, &rendererTexture);
}

void ForwardRenderer::setup() {
    glGenFramebuffers(1, &rendererFBO);
    glGenRenderbuffers(1, &rendererRBO);
    glGenTextures(1, &rendererTexture);

	glGenFramebuffers(1, &depthInfoFBO);
    glGenRenderbuffers(1, &depthInfoRBO);
    glGenTextures(1, &depthInfoPositionTexture);
	glGenTextures(1, &depthInfoNormalTexture);

	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, getRendererId(), uboMatrices, 0, 2 * sizeof(glm::mat4) + sizeof(GLfloat));

	hdrFilter.setup();
	ssaoFilter.setup();


	float rectangle[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
	};
	
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), &rectangle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(float)));
	glBindVertexArray(0);



    shaderMesh = ResourceManager::loadShader(ShaderName::Renderer::Forward::Basic::Mesh);
}

ForwardSceneObject * ForwardRenderer::generateForwardObject(SceneObject * sceneObject) {
	ForwardSceneObject * newForwardObject = nullptr;

	if(auto mesh = dynamic_cast<Mesh*>(sceneObject)) {
		newForwardObject = new ForwardMesh(mesh);
	}
	else if(auto text = dynamic_cast<Text*>(sceneObject)) {
		newForwardObject = new ForwardText(text);

	}
	else if(auto particle = dynamic_cast<ParticleGenerator*>(sceneObject)) {
		if(auto point2D = dynamic_cast<PointGenerator2D*>(particle)) {
			if(auto billboard2D = dynamic_cast<BillboardGenerator2D*>(particle)) {
				newForwardObject = new ForwardBillboardGenerator2D(billboard2D);
			}
			else {				
				newForwardObject = new ForwardPointGenerator2D(point2D);
			}
		}
		else if(auto point3D = dynamic_cast<PointGenerator3D*>(particle)) {
			if(auto billboard3D = dynamic_cast<BillboardGenerator3D*>(point3D)) {
				newForwardObject = new ForwardBillboardGenerator3D(billboard3D);
			}
			else {
				newForwardObject = new ForwardPointGenerator3D(point3D);
			}
		}
		else {
			newForwardObject = new ForwardSceneObject(sceneObject);
		}
	}
	else {
		newForwardObject = new ForwardSceneObject(sceneObject);
	}

	for(auto child : sceneObject->getChildren()) {
		newForwardObject->children.push_back(static_cast<RenderObject*>(generateForwardObject(child)));
	}
	newForwardObject->update();
	return newForwardObject;	
}
