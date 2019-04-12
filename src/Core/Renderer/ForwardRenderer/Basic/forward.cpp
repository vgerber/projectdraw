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

	hdrFilter.resize(getWidth(), getHeight());
}

void ForwardRenderer::clearScreen() {
    glBindFramebuffer(GL_FRAMEBUFFER, rendererFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void ForwardRenderer::render() {
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

	//GLcheckError();

	glUniformBlockBinding(shaderMesh.getId(), glGetUniformBlockIndex(shaderMesh.getId(), "Matrices"), getRendererId());
	float farZ = camera->getClippingFar();
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera->getViewMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->getProjectionMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(GLfloat), &farZ);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, rendererFBO);
	glViewport(0, 0, getWidth(), getHeight());

	glm::vec3 viewPos = camera->getWorldTransform().getTranslation();
	glUniform3f(glGetUniformLocation(shaderMesh.getId(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);

    for(auto drawable : drawables) {
		drawable->setCameraMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
        renderDrawableRaw(drawable, shaderMesh, drawable->settings.drawType);
    }

	hdrFilter.apply(rendererTexture);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);	
}

void ForwardRenderer::addSceneObject(SceneObject &sceneObject) {

	if(DirectionalLight * dirLight = dynamic_cast<DirectionalLight*>(&sceneObject)) {
		this->directionalLight = dirLight;
	}

	if (Drawable *drawable = dynamic_cast<Drawable *>(&sceneObject))
	{
		drawables.push_back(drawable);
		return;
	}

	printf("[Engine] [DeferredRenderer] [Error] Scene doesn't accept %s\n", sceneObject.getId().c_str());
}

void ForwardRenderer::removeSceneObject(SceneObject &sceneObject) {

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

	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, getRendererId(), uboMatrices, 0, 2 * sizeof(glm::mat4) + sizeof(GLfloat));

	hdrFilter.setup();

    shaderMesh = ResourceManager::loadShader(ShaderName::Renderer::Forward::Basic::Mesh);
}

void ForwardRenderer::renderDrawableRaw(Drawable * drawable, Shader shader,  DrawType drawType) {
	if(drawable) {
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(drawable->getWorldTransform().getMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"),   1, GL_FALSE, glm::value_ptr(drawable->getMVPMatrix()));

		glUniform1i(glGetUniformLocation(shader.getId(), "alphaTexture"),    0);
		glUniform1i(glGetUniformLocation(shader.getId(), "diffuseTexture"),  1);
		glUniform1i(glGetUniformLocation(shader.getId(), "specularTexture"), 2);
		if(Text * textObject = dynamic_cast<Text*>(drawable)) {
			glUniform1i(glGetUniformLocation(shader.getId(), "enableAlphaTexture"),    1);
			glUniform1i(glGetUniformLocation(shader.getId(), "enableDiffuseTexture"),  0);
			glUniform1i(glGetUniformLocation(shader.getId(), "enableSpecularTexture"), 0);
			for(int i = 0; i < textObject->getText().size(); i++) {
				glActiveTexture(GL_TEXTURE0);
				//glBindTexture(GL_TEXTURE_2D, textObject->activateChar(i));
				textObject->draw(drawType);
			}
		}
		else {
			glUniform1i(glGetUniformLocation(shader.getId(), "enableAlphaTexture"),    0);
			glUniform1i(glGetUniformLocation(shader.getId(), "enableDiffuseTexture"),  0);
			glUniform1i(glGetUniformLocation(shader.getId(), "enableSpecularTexture"), 0);
			drawable->draw(drawType);
		}

		for(auto child : drawable->getChildren()) {
			renderDrawableRaw(dynamic_cast<Drawable*>(child), shader, drawType);
		}
	}
	//GLcheckError();
}

