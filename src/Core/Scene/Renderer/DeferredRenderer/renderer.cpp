#include "renderer.h"

DeferredRenderer::DeferredRenderer(int width, int height, Camera &camera) : AbstractRenderer(width, height, camera)
{
    setup();
    resize(width, height);
}

void DeferredRenderer::clearScreen() {
	glBindFramebuffer(GL_FRAMEBUFFER, screenRectFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void DeferredRenderer::render()
{
	glEnable(GL_CULL_FACE);
	if (invalidShaders) {
		refreshShaderRenderer();
		invalidShaders = false;
	}

	glViewport(0, 0, getWidth(), getHeight());
	glEnable(GL_DEPTH_TEST);


	renderObjects();

	renderLight();

	applyBloom();

	applyAntialias();

	//applyHDR();
}

void DeferredRenderer::addSceneObject(SceneObject & sceneObject)
{
	if (DirectionalLight *dLight = dynamic_cast<DirectionalLight *>(&sceneObject))
	{
		directionalLight = dLight;
		return;
	}

	if (Animatable *animatable = dynamic_cast<Animatable *>(&sceneObject))
	{
		animatables.push_back(animatable);
		return;
	}

	if (Instancer *instancer = dynamic_cast<Instancer *>(&sceneObject))
	{
		instancers.push_back(instancer);
		return;
	}

	if (SpotLight *sLight = dynamic_cast<SpotLight *>(&sceneObject))
	{
		spotLights.push_back(sLight);
		return;
	}

	if (PointLight *pLight = dynamic_cast<PointLight *>(&sceneObject))
	{
		pointLights.push_back(pLight);
		return;
	}

	if (Drawable *drawable = dynamic_cast<Drawable *>(&sceneObject))
	{
		objects.push_back(drawable);
	/*
		if (dynamic_cast<Text*>(drawable))
			drawable->setShader(shaderFont, *this);
		else if (dynamic_cast<ParticleGenerator*>(drawable))
			drawable->setShader(shaderInstancing, *this);
		else
			drawable->setShader(shaderBasic, *this);
	*/
		return;
	}

	printf("[Engine] [DeferredRenderer] [Error] Scene doesn't accept %s\n", sceneObject.getId().c_str());
}

void DeferredRenderer::removeSceneObject(SceneObject & sceneObject)
{
	if (Drawable *drawable = dynamic_cast<Drawable *>(&sceneObject))
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i] == drawable)
			{
				objects.erase(objects.begin() + i);
				i--;
			}
		}
		return;
	}

	if (DirectionalLight *dLight = dynamic_cast<DirectionalLight *>(&sceneObject))
	{
		directionalLight = dLight;
		return;
	}

	if (Animatable *animatable = dynamic_cast<Animatable *>(&sceneObject))
	{
		for (size_t i = 0; i < animatables.size(); i++)
		{
			if (animatables[i] == animatable)
			{
				animatables.erase(animatables.begin() + i);
				i--;
			}
		}
		return;
	}

	if (Instancer *instancer = dynamic_cast<Instancer *>(&sceneObject))
	{
		instancers.push_back(instancer);
		return;
	}

	if (SpotLight *sLight = dynamic_cast<SpotLight *>(&sceneObject))
	{
		spotLights.push_back(sLight);
		return;
	}

	if (PointLight *pLight = dynamic_cast<PointLight *>(&sceneObject))
	{
		pointLights.push_back(pLight);
		return;
	}

	printf("... doesn't accept %s\n", "OBJECT");
}

GLuint DeferredRenderer::getTexture() {
	return screenRectTexture;
}

void DeferredRenderer::resize(int width, int height) {
	AbstractRenderer::resize(width, height);
	/*
	Deferred Rendering Textures
	*/
	//gBuffer
	{
		glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
		glBindTexture(GL_TEXTURE_2D, gBufferPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, getWidth(), getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gBufferPosition, 0);
		/*glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, gBufferPosition);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, AASamples, GL_RGBA16F, getWidth(), getHeight(), GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, gBufferPosition, 0);*/

		glBindTexture(GL_TEXTURE_2D, gBufferNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, getWidth(), getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gBufferNormal, 0);
		/*glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, gBufferNormal);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, AASamples, GL_RGBA16F, getWidth(), getHeight(), GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, gBufferNormal, 0);*/

		glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, getWidth(), getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gBufferAlbedo, 0);
		/*glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, gBufferAlbedo);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, AASamples, GL_RGBA16F, getWidth(), getHeight(), GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D_MULTISAMPLE, gBufferAlbedo, 0);*/

		glBindTexture(GL_TEXTURE_2D, gBufferOption1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getWidth(), getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gBufferOption1, 0);
		/*glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, gBufferOption1);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, AASamples, GL_RGBA, getWidth(), getHeight(), GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D_MULTISAMPLE, gBufferOption1, 0);*/

		glBindTexture(GL_TEXTURE_2D, gBufferGlow);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, getWidth(), getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gBufferGlow, 0);
		/*glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, gBufferGlow);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, AASamples, GL_RGBA16F, getWidth(), getHeight(), GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D_MULTISAMPLE, gBufferGlow, 0);*/

		GLuint gAttachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		glDrawBuffers(5, gAttachments);

		glBindRenderbuffer(GL_RENDERBUFFER, rboGDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, getWidth(), getHeight());
		//glRenderbufferStorageMultisample(GL_RENDERBUFFER, AASamples, GL_DEPTH24_STENCIL8, getWidth(), getHeight());
		// - Attach buffers

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboGDepth);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//Bloom
	{
		glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);


		for (int i = 0; i < bloomSample; i++) {
			glBindTexture(GL_TEXTURE_2D, bloomTextures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, getWidth() / pow(2, i), getHeight() / pow(2, i), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomTextures[i], 0);

			glBindTexture(GL_TEXTURE_2D, tmpbloomTextures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, getWidth() / pow(2, i), getHeight() / pow(2, i), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	/*
	Screen Texture
	*/
	{
		glBindFramebuffer(GL_FRAMEBUFFER, screenRectFBO);
		glBindTexture(GL_TEXTURE_2D, screenRectTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, getWidth(), getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, screenRectTexture);
		//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, AASamples, GL_RGBA16F, getWidth(), getHeight(), GL_TRUE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenRectTexture, 0);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, screenRectTexture, 0);
		GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glBindTexture(GL_TEXTURE_2D, tmpRenderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, getWidth(), getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	/*glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tmpRenderTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, AASamples, GL_RGBA16F, getWidth(), getHeight(), GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);*/
}

void DeferredRenderer::renderObjects()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//sort objects for stencil testing
	//std::sort(objects.begin(), objects.end(), SortDrawable(camera->getPosition()));

	glUniformBlockBinding(shaderBasic.getId(), glGetUniformBlockIndex(shaderBasic.getId(), "Matrices"), getRendererId());
	glUniformBlockBinding(shaderFont.getId(), glGetUniformBlockIndex(shaderFont.getId(), "Matrices"), getRendererId());
	glUniformBlockBinding(shaderLight.getId(), glGetUniformBlockIndex(shaderLight.getId(), "Matrices"), getRendererId());
	glUniformBlockBinding(shaderInstancing.getId(), glGetUniformBlockIndex(shaderInstancing.getId(), "Matrices"), getRendererId());
	glUniformBlockBinding(shaderNormals.getId(), glGetUniformBlockIndex(shaderNormals.getId(), "Matrices"), getRendererId());


	int polygonMode = GL_FILL;
	if (renderMode == RenderMode::POINTR)
		polygonMode = GL_POINT;
	else if (renderMode == RenderMode::LINER)
		polygonMode = GL_LINE;
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	/*glBindFramebuffer(GL_READ_FRAMEBUFFER, gBufferFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, getWidth(), getHeight(), 0, 0, getWidth(), getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);*/
	glViewport(0, 0, getWidth(), getHeight());

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glStencilMask(0x00);

	//bind global camera matrices and properties
	float farZ = camera->getClippingFar();
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera->getViewMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->getProjectionMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(GLfloat), &farZ);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//draw 3d pointlight object
	/*shaderLight.use();
	for (auto plight : pointLights)
	{
		glUniform1f(glGetUniformLocation(shaderLight.getId(), "intensity"), plight->intensity);
		glUniform3f(glGetUniformLocation(shaderLight.getId(), "color"), plight->diffuse.r, plight->diffuse.g, plight->diffuse.b);
		plight->setCameraMatrices(camera->getViewMatrix(), camera->getCameraMatrix());
		plight->draw();
	}*/

	shaderInstancing.use();
	for (auto instancer : instancers)
	{
		//instancer->draw(shaderInstancing);
	}

	//draw particles

	//draw all drawables
	shaderBasic.use();
	for (auto drawable : objects)
	{
		drawable->setCameraMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
		renderDrawable(drawable);
	}
	//draw viewfrustums from cameras
	/*for (auto &scam : cameras)
	{
		if (&scam != &sceneCamera && scam.config.debugVisible)
		{
			Geometry geoCam = scam.getDebugViewFrustum(directionalLight->getCSMSlices());
			geoCam.draw();
			geoCam.dispose();
		}
	}*/

	//draw debug normals of drawables
	shaderNormals.use();
	for (auto drawable : objects)
	{
		if (drawable->settings.normalVisible)
		{
			renderDrawableNormals(drawable);
		}
	}
	//draw bounding box of drawable (not aabb)
	shaderBasic.use();
	for (auto drawable : objects)
	{
		if (drawable->settings.boxVisible)
		{
			renderDrawableBox(drawable);
		}
	}

	//draw aabb from drawables
	/*
	shaderGeometry.use();
	glUniform4f(glGetUniformLocation(shaderGeometry.getId(), "color"), 0.0f, 1.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shaderGeometry.getId(), "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	*/
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::renderDrawable(Drawable * drawable, DrawType drawType) {
	DrawableInfo settings = drawable->settings;
	if (settings.xrayVisible) {
		//glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
		//glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(cameraProj * cameraView * getModelMatrix()));
		glUniform1f(glGetUniformLocation(shaderBasic.getId(), "useLight"), settings.xrayUseLight);
		glUniform1i(glGetUniformLocation(shaderBasic.getId(), "enableCustomColor"), settings.xrayCustomColor);
		glm::vec4 color = settings.xrayColor;
		glUniform4f(glGetUniformLocation(shaderBasic.getId(), "customColor"), color.r, color.g, color.b, color.a);

		glEnable(GL_DEPTH_TEST);
		//set all occupied pixels to 1
		glStencilFunc(GL_GREATER, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_REPLACE, GL_KEEP);
		glStencilMask(0xFF);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		renderDrawableRaw(drawable, settings.xrayDrawType);

		//draw to all pixels with stencil equals 1 and reset it to zero
		glStencilFunc(GL_EQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilMask(0xFF);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glDisable(GL_DEPTH_TEST);
		renderDrawableRaw(drawable, settings.xrayDrawType);

		glEnable(GL_DEPTH_TEST);
	}


	if (settings.outlineVisible) {
		glUniform1f(glGetUniformLocation(shaderBasic.getId(), "useLight"), 0.0f);
		glUniform1i(glGetUniformLocation(shaderBasic.getId(), "enableCustomColor"), 1);
		glm::vec4 color = settings.outlineColor;
		glUniform4f(glGetUniformLocation(shaderBasic.getId(), "customColor"), color.r, color.g, color.b, color.a);

		//set all occupied pixels to 2
		glStencilFunc(GL_GREATER, 2, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		renderDrawableRaw(drawable, DrawType::TRIANGLEG);


		Size outlineSize;
		Size size = drawable->getSize();
		float thickness = settings.outlineThickness;
		outlineSize.width = (size.width + thickness) / size.width;
		outlineSize.height = (size.height + thickness) / size.height;
		outlineSize.depth = (size.depth + thickness) / size.depth;

		glm::vec3 oldScale = drawable->getScale();
		glm::vec3 oldPosition = drawable->getPosition();

		drawable->scale(outlineSize.width, outlineSize.height, outlineSize.depth);

		
		//setPosition(getPosition() + glm::vec3(-0.5f * thickness));

		glStencilFunc(GL_EQUAL, 0, 0xFF);
		glStencilOp(GL_ZERO, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);

		renderDrawableRaw(drawable, DrawType::TRIANGLEG);

		drawable->setPosition(oldPosition);
		drawable->scale(oldScale);
	}

	glUniform1f(glGetUniformLocation(shaderBasic.getId(), "useLight"), settings.useLight);
	glUniform1i(glGetUniformLocation(shaderBasic.getId(), "enableCustomColor"), settings.useCustomColor);
	glm::vec4 color = settings.customColor;
	glUniform4f(glGetUniformLocation(shaderBasic.getId(), "customColor"), color.r, color.g, color.b, color.a);

	glStencilFunc(GL_EQUAL, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	renderDrawableRaw(drawable, settings.drawType);
}

void DeferredRenderer::renderDrawableRaw(Drawable * drawable, DrawType drawType) {
	if(drawable) {
		glUniformMatrix4fv(glGetUniformLocation(shaderBasic.getId(), "model"), 1, GL_FALSE, glm::value_ptr(drawable->getWorldTransform().getMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shaderBasic.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(drawable->getMVPMatrix()));

		glUniform1i(glGetUniformLocation(shaderBasic.getId(), "diffuseTexture"), 0);
		glUniform1i(glGetUniformLocation(shaderBasic.getId(), "specularTexture"), 1);
		if(Text * textObject = dynamic_cast<Text*>(drawable)) {
			glUniform1i(glGetUniformLocation(shaderBasic.getId(), "enableDiffuseTexture"), 1);
			glUniform1i(glGetUniformLocation(shaderBasic.getId(), "enableSpecularTexture"), 0);
			for(int i = 0; i < textObject->getText().size(); i++) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textObject->activateChar(i));
				textObject->draw(drawType);
			}
		}
		else {
			glUniform1i(glGetUniformLocation(shaderBasic.getId(), "enableDiffuseTexture"), 0);
			glUniform1i(glGetUniformLocation(shaderBasic.getId(), "enableSpecularTexture"), 0);
			drawable->draw(drawType);
		}

		for(auto child : drawable->getChildren()) {
			renderDrawableRaw(dynamic_cast<Drawable*>(child), drawType);
		}
	}
}

void DeferredRenderer::renderDrawableNormals(Drawable * drawable) {
	if(drawable) {
		glUniformMatrix4fv(glGetUniformLocation(shaderNormals.getId(), "model"), 1, GL_FALSE, glm::value_ptr(drawable->getWorldTransform().getMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shaderNormals.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(drawable->getMVPMatrix()));
		drawable->draw(DrawType::TRIANGLEG);
		for(auto child : drawable->getChildren()) {
			renderDrawableNormals(dynamic_cast<Drawable*>(child));
		}
	}
}

void DeferredRenderer::renderDrawableBox(Drawable * drawable) {
	if(drawable) {
		glUniformMatrix4fv(glGetUniformLocation(shaderBasic.getId(), "model"), 1, GL_FALSE, glm::value_ptr(drawable->getWorldTransform().getMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shaderBasic.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(drawable->getMVPMatrix()));
		drawable->drawBox();
		for(auto child : drawable->getChildren()) {
			renderDrawableBox(dynamic_cast<Drawable*>(child));
		}
	}
}

void DeferredRenderer::renderLight()
{
	/*
	Shadowmapping:
		1. Viewfrustim DirLight fro CSM
		2. Shadowmapping (SM) DirLight
		3. SM Point
		4. SM Spot
	Rendering:
		1. Dir
		2. Point
		3. Spot
		4. All off (=Albedo)	


		tmpRenderTexture is a full scene size buffer for tmp save of current screen pixels (e.g. += pixel operations which will otherwise fail)
		this texture has to be prefilled or filled (black) or with light renderering (p and s light will need them)
	*/

	//sort spot- and pointlights for shadow pipeline
	std::sort(pointLights.begin(), pointLights.end(), SortPointLights(camera->getPosition()));
	std::sort(spotLights.begin(), spotLights.end(), SortSpotLights(camera->getPosition()));
	
	

	if (directionalLight && directionalLight->shadow)
	{
		//set viewfrustum for directional light (for cascaded shadow mapping)
		directionalLight->setViewFrustum(camera->getViewFrustum(directionalLight->getCSMSlices()));

		//render csm directionLight
		for (int i = 0; i < directionalLight->getCSMSlices(); i++)
		{
			directionalLight->beginShadowMapping(i);

			for (auto drawable : objects)
			{
				renderDrawableRaw(drawable, DrawType::TRIANGLEG);
			}
			directionalLight->endShadowMapping();
		}
	}

	//count shadows for each light for shadow render pipeline
	int pointLightShadowCount = 0;
	int spotLightShadowCount = 0;
	
	//Point Light Shadow
	
	if (pointLights.size() > 0)
	{
		for (auto plight : pointLights)
		{
			//reduce depth cubemap generation to actual shadow maximum
			if(pointLightShadowCount == PointLightShadows) {
				break;
			}	
			//avoid shadow mapping for non visible lights
			if (plight->intensity == 0.0) {
				continue;
			}

			if (plight->shadow) {				
				plight->beginShadowMapping();
				for (auto drawable : objects)
				{
					if(glm::length(drawable->getPosition() - plight->getPosition()) < plight->getDistance()) {
						//draw each drawable to depth buffer if it is within the distance of the light and apply 
						//the corresponding depth shader
						renderDrawableRaw(drawable, DrawType::TRIANGLEG);
					}
				}
				plight->endShadowMapping();
			}
			else {
				//if the list is correctly sorted 
				//the first non shadow light marks the beginning of non shadow lights
				break;
			}
			pointLightShadowCount++;
		}
	}

	//SpotLight Shadow
	if (spotLights.size() > 0) {
		for (auto slight : spotLights) {

			//stop if maximum of shadows is reached
			if(spotLightShadowCount == SpotLightShadows) {
				break;
			}

			//avoid shadow mapping for non visible lights
			if (slight->intensity == 0.0) {
				continue;
			}

			if (slight->shadow) {
				slight->beginShadowMapping();
				for (auto drawable : objects)
				{
					if(slight->getDistance() > glm::length(drawable->getPosition() - slight->getPosition())) {
						//draw each drawable to depth buffer if it is within the distance of the light and apply 
						//the corresponding depth shader
						renderDrawableRaw(drawable, DrawType::TRIANGLEG);
					}
				}
				slight->endShadowMapping();
			}
			else {
				//stop if first non shadow light was encounterd
				break;
			}
			spotLightShadowCount++;
		}
	}


	glm::vec3 viewPos = camera->getPosition();

	

	glBindFramebuffer(GL_FRAMEBUFFER, screenRectFBO);
	glViewport(0, 0, getWidth(), getHeight());
	
	if (directionalLight)
	{
		shaderDLightShadow.use();

		glUniform3f(glGetUniformLocation(shaderDLightShadow.getId(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);

		glUniform1i(glGetUniformLocation(shaderDLightShadow.getId(), "gPosition"), 0);
		glUniform1i(glGetUniformLocation(shaderDLightShadow.getId(), "gNormal"),   1);
		glUniform1i(glGetUniformLocation(shaderDLightShadow.getId(), "gAlbedo"),   2);
		glUniform1i(glGetUniformLocation(shaderDLightShadow.getId(), "gOption1"),  3);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBufferPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gBufferNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gBufferOption1);

		setDirectionalLightUniforms(shaderDLightShadow, *directionalLight, 4);
		
		//set back to fill mode for texture rendering
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		glCopyTextureSubImage2D(tmpRenderTexture, 0, 0, 0, 0, 0, getWidth(), getHeight());
	}
	else {
		glCopyTextureSubImage2D(tmpRenderTexture, 0, 0, 0, 0, 0, getWidth(), getHeight());
	}


	
	/*
	Point Light

	1. Light + Shadow
	2. Light - Shadow
	*/	

	if(pointLightShadowCount > 0) {

		shaderPLightShadow.use();
		glUniform3f(glGetUniformLocation(shaderPLightShadow.getId(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);

		glUniform1i(glGetUniformLocation(shaderPLightShadow.getId(), "gPosition"), 0);
		glUniform1i(glGetUniformLocation(shaderPLightShadow.getId(), "gNormal"), 1);
		glUniform1i(glGetUniformLocation(shaderPLightShadow.getId(), "gAlbedo"), 2);
		glUniform1i(glGetUniformLocation(shaderPLightShadow.getId(), "gOption1"), 3);
		glUniform1i(glGetUniformLocation(shaderPLightShadow.getId(), "prevTexture"), 4);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBufferPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gBufferNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gBufferOption1);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, tmpRenderTexture);

		int pLightCount = 0;
		for(int pLightIndex = 0; pLightIndex < pointLightShadowCount; pLightIndex++) {
			PointLight * pLight = pointLights[pLightIndex];

			if(pLight->intensity == 0.0) {
				continue;
			}

			//set pointlight uniforms		
			setPointLightUniforms(shaderPLightShadow, *pLight, 5, pLightCount);

			pLightCount++;
			//draw current set point ligts if array limit or last index is reached
			if(pLightCount == ShaderPointLightShadowSize || pLightIndex == pointLightShadowCount-1) {
				glUniform1i(glGetUniformLocation(shaderPLightShadow.getId(), "pointLights"), pLightCount);
			
				glBindVertexArray(screenRectVAO);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				glBindVertexArray(0);

				glCopyTextureSubImage2D(tmpRenderTexture, 0, 0, 0, 0, 0, getWidth(), getHeight());
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, tmpRenderTexture);

				pLightCount = 0;
			}
		}
	}

	if (pointLights.size() > 0 && pointLights.size() != pointLightShadowCount)
	{
		shaderPLight.use();
		glUniform3f(glGetUniformLocation(shaderPLight.getId(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);

		glUniform1i(glGetUniformLocation(shaderPLight.getId(), "gPosition"), 0);
		glUniform1i(glGetUniformLocation(shaderPLight.getId(), "gNormal"), 1);
		glUniform1i(glGetUniformLocation(shaderPLight.getId(), "gAlbedo"), 2);
		glUniform1i(glGetUniformLocation(shaderPLight.getId(), "gOption1"), 3);
		glUniform1i(glGetUniformLocation(shaderPLight.getId(), "prevTexture"), 4);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBufferPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gBufferNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gBufferOption1);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, tmpRenderTexture);


		int pLightCount = 0;
		for (int pLightIndex = pointLightShadowCount; pLightIndex < pointLights.size(); pLightIndex++)
		{
			PointLight * pLight = pointLights[pLightIndex];

			if(pLight->intensity == 0.0) {
				continue;
			}

			setPointLightUniforms(shaderPLight, *pLight, 5, pLightCount);
			pLightCount++;

			//draw lights if maximum of array or last element is reached
			if(pLightCount == ShaderPointLightSize || pLightIndex == pointLights.size()-1) {
				glUniform1i(glGetUniformLocation(shaderPLight.getId(), "pointLights"), pLightCount);
				
				glBindVertexArray(screenRectVAO);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				glBindVertexArray(0);

				glCopyTextureSubImage2D(tmpRenderTexture, 0, 0, 0, 0, 0, getWidth(), getHeight());
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, tmpRenderTexture);
				pLightCount = 0;
			}
		}

		
	}

	/*
	Spotlight Rendering

	1. Light + Shadow
	2. Light - Shadow
	*/
	if (spotLightShadowCount > 0)
	{
		shaderSLightShadow.use();
		glUniform3f(glGetUniformLocation(shaderSLightShadow.getId(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);

		glUniform1i(glGetUniformLocation(shaderSLightShadow.getId(), "gPosition"), 0);
		glUniform1i(glGetUniformLocation(shaderSLightShadow.getId(), "gNormal"), 1);
		glUniform1i(glGetUniformLocation(shaderSLightShadow.getId(), "gAlbedo"), 2);
		glUniform1i(glGetUniformLocation(shaderSLightShadow.getId(), "gOption1"), 3);
		glUniform1i(glGetUniformLocation(shaderSLightShadow.getId(), "prevTexture"), 4);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBufferPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gBufferNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gBufferOption1);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, tmpRenderTexture);

		int sLightCount = 0;
		for(int sLightIndex = 0; sLightIndex < spotLightShadowCount; sLightIndex++) {
			SpotLight * sLight = spotLights[sLightIndex];

			if(sLight->intensity == 0.0) {
				continue;
			}

			setSpotLightUniforms(shaderSLightShadow, *sLight, 5 + sLightCount, sLightCount);

			sLightCount++;
			if(sLightCount == ShaderSpotLightShadowSize || sLightIndex == spotLightShadowCount-1) {
				glUniform1i(glGetUniformLocation(shaderSLightShadow.getId(), "spotLights"), sLightCount);

				glBindVertexArray(screenRectVAO);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				glBindVertexArray(0);

				glCopyTextureSubImage2D(tmpRenderTexture, 0, 0, 0, 0, 0, getWidth(), getHeight());
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, tmpRenderTexture);
				sLightCount = 0;
			}
		}
	}
	//render spotlights without shadowmapping
	if (spotLights.size() > 0 && spotLights.size() != spotLightShadowCount)
	{
		shaderSLight.use();
		glUniform3f(glGetUniformLocation(shaderSLight.getId(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);

		glUniform1i(glGetUniformLocation(shaderSLight.getId(), "gPosition"), 0);
		glUniform1i(glGetUniformLocation(shaderSLight.getId(), "gNormal"), 1);
		glUniform1i(glGetUniformLocation(shaderSLight.getId(), "gAlbedo"), 2);
		glUniform1i(glGetUniformLocation(shaderSLight.getId(), "gOption1"), 3);
		glUniform1i(glGetUniformLocation(shaderSLight.getId(), "prevTexture"), 4);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBufferPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gBufferNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gBufferOption1);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, tmpRenderTexture);

		int sLightCount = 0;
		for(int sLightIndex = spotLightShadowCount; sLightIndex < spotLights.size(); sLightIndex++) {
			SpotLight * sLight = spotLights[sLightIndex];
			setSpotLightUniforms(shaderSLight, *sLight, 5, sLightCount);

			if(sLight->intensity == 0.0) {
				continue;
			}

			sLightCount++;
			if(sLightCount == ShaderSpotLightSize || sLightIndex == spotLights.size()-1) {
				glUniform1i(glGetUniformLocation(shaderSLight.getId(), "spotLights"), sLightCount);

				glBindVertexArray(screenRectVAO);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				glBindVertexArray(0);

				glCopyTextureSubImage2D(tmpRenderTexture, 0, 0, 0, 0, 0, getWidth(), getHeight());
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, tmpRenderTexture);
				sLightCount = 0;
			}
		}
	}




	//render just on camera textrue if no lights active
	if (!directionalLight && spotLights.size() == 0 && pointLights.size() == 0)
	{
		shaderTexture.use();

		glUniform1i(glGetUniformLocation(shaderTexture.getId(), "screenTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);

		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		glCopyTextureSubImage2D(tmpRenderTexture, 0, 0, 0, 0, 0, getWidth(), getHeight());
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::applyAntialias()
{
	glBindFramebuffer(GL_FRAMEBUFFER, screenRectFBO);
	glCopyTextureSubImage2D(tmpRenderTexture, 0, 0, 0, 0, 0, getWidth(), getHeight());

	shaderFXAA.use();

	glUniform1i(glGetUniformLocation(shaderFXAA.getId(), "screenTexture"), 0);
	glUniform2f(glGetUniformLocation(shaderFXAA.getId(), "inverseScreenSize"), 1.0f / getWidth(), 1.0f / getHeight());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tmpRenderTexture);

	glBindVertexArray(screenRectVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::applyBloom()
{
	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
	
	//render to scaled textures
	shaderBloomBlur.use();
	GLuint offsetLocation = glGetUniformLocation(shaderBloomBlur.getId(), "offset");


	for (int i = 0; i < bloomSample; i++) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomTextures[i], 0);

		//draw glowBuffer to bloom images
		shaderTexture.use();
		glUniform1i(glGetUniformLocation(shaderTexture.getId(), "screenTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBufferGlow);

		int tmpWidth = getWidth() / pow(2, i);
		int tmpHeight = getHeight() / pow(2, i);

		glViewport(0, 0, tmpWidth, tmpHeight);


		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		glCopyTextureSubImage2D(tmpbloomTextures[i], 0, 0, 0, 0, 0, tmpWidth, tmpHeight);

		
		//blur glow image
		shaderBloomBlur.use();

		glUniform1i(glGetUniformLocation(shaderBloomBlur.getId(), "glowTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tmpbloomTextures[i]);

		glUniform2f(offsetLocation, 0, 1.2 / getHeight());
		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		glCopyTextureSubImage2D(tmpbloomTextures[i], 0, 0, 0, 0, 0, tmpWidth, tmpHeight);

		glUniform2f(offsetLocation, 1.2 / getWidth(), 0);
		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);


	}
	
	
	//Merge glow textures
	glBindFramebuffer(GL_FRAMEBUFFER, screenRectFBO);
	glCopyTextureSubImage2D(tmpRenderTexture, 0, 0, 0, 0, 0, getWidth(), getHeight());
	glViewport(0, 0, getWidth(), getHeight());
	shaderBloomMerge.use();
	
	glUniform1i(glGetUniformLocation(shaderBloomMerge.getId(), "screenTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tmpRenderTexture);

	
	for (int i = 0; i < bloomSample; i++) {
		std::string glowStr = "glow" + std::to_string((int)pow(2, i));
		glUniform1i(glGetUniformLocation(shaderBloomMerge.getId(), glowStr.c_str()), 1 + i);
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, bloomTextures[i]);
	}
	

	glBindVertexArray(screenRectVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	
	glCopyTextureSubImage2D(screenRectTexture, 0, 0, 0, 0, 0, getWidth(), getHeight());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::applyHDR()
{
	glBindFramebuffer(GL_FRAMEBUFFER, screenRectFBO);
	glCopyTextureSubImage2D(tmpRenderTexture, 0, 0, 0, 0, 0, getWidth(), getHeight());

	shaderHDR.use();

	glUniform1i(glGetUniformLocation(shaderFXAA.getId(), "screenTexture"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tmpRenderTexture);

	glBindVertexArray(screenRectVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void DeferredRenderer::dispose() {
	glDeleteFramebuffers(1, &gBufferFBO);
	glDeleteRenderbuffers(1, &rboGDepth);
	glDeleteTextures(1, &gBufferAlbedo);
	glDeleteTextures(1, &gBufferNormal);
	glDeleteTextures(1, &gBufferPosition);
	glDeleteTextures(1, &gBufferOption1);

	glDeleteFramebuffers(1, &screenRectFBO);
	glDeleteVertexArrays(1, &screenRectVAO);
	glDeleteBuffers(1, &screenRectVBO);
	glDeleteTextures(1, &screenRectTexture);

	glDeleteTextures(1, &tmpRenderTexture);
}

void DeferredRenderer::refreshShaderRenderer()
{
	/*for (auto &drawable : objects) {
		if (dynamic_cast<Text*>(drawable))
			drawable->setShader(shaderFont, *this);
		else if (dynamic_cast<ParticleGenerator*>(drawable))
			drawable->setShader(shaderInstancing, *this);
		else
			drawable->setShader(shaderBasic, *this);
	}*/
}

int DeferredRenderer::getRendererType()
{
	return RendererType;
}

void DeferredRenderer::setup() {
	glEnable(GL_MULTISAMPLE);
	renderMode = RenderMode::FILLR;

	glGenFramebuffers(1, &gBufferFBO);
	glGenFramebuffers(1, &bloomFBO);
	glGenFramebuffers(1, &screenRectFBO);

	glGenTextures(1, &gBufferPosition);
	glGenTextures(1, &gBufferNormal);
	glGenTextures(1, &gBufferAlbedo);
	glGenTextures(1, &gBufferOption1);
	glGenTextures(1, &gBufferGlow);
	glGenTextures(1, &screenRectTexture);
	glGenTextures(1, &tmpRenderTexture);
	

	for (int i = 0; i < bloomSample; i++) {
		glGenTextures(1, &bloomTextures[i]);
		glGenTextures(1, &tmpbloomTextures[i]);
	}


	glGenRenderbuffers(1, &rboGDepth);

	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, getRendererId(), uboMatrices, 0, 2 * sizeof(glm::mat4) + sizeof(GLfloat));

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

	shaderDLightShadow = ResourceManager::loadShader(ShaderName::Deferred::Pipeline::Shadow::D);
	shaderPLight = ResourceManager::loadShader(ShaderName::Deferred::Pipeline::P);
	shaderPLightShadow = ResourceManager::loadShader(ShaderName::Deferred::Pipeline::Shadow::P);
	shaderSLight = ResourceManager::loadShader(ShaderName::Deferred::Pipeline::S);
	shaderSLightShadow = ResourceManager::loadShader(ShaderName::Deferred::Pipeline::Shadow::S);
	shaderTexture = ResourceManager::loadShader(ShaderName::Deferred::Pipeline::Texture::ScreenTexture);

	shaderBasic = ResourceManager::loadShader(ShaderName::Deferred::Mesh::Basic);
	shaderLight = ResourceManager::loadShader(ShaderName::Deferred::Mesh::Light);
	shaderFont = ResourceManager::loadShader(ShaderName::Deferred::Mesh::Font);	

	shaderNormals = ResourceManager::loadShader(ShaderName::Deferred::Debug::Normal);

	shaderFXAA = ResourceManager::loadShader(ShaderName::Postprocessing::Antialias::FXAA);
	shaderHDR = ResourceManager::loadShader(ShaderName::Postprocessing::HDR::Basic);
	shaderBloomBlur = ResourceManager::loadShader(ShaderName::Postprocessing::Bloom::Blur);
	shaderBloomMerge = ResourceManager::loadShader(ShaderName::Postprocessing::Bloom::Merge);
}