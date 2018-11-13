#include "r_deferred.h"

DeferredRenderer::DeferredRenderer(int width, int height, Camera &camera) : AbstractRenderer(width, height, camera)
{
    setup();
    resize(width, height);
}

void DeferredRenderer::clearScreen() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void DeferredRenderer::render()
{
	glViewport(0, 0, getWidth(), getHeight());
	glEnable(GL_DEPTH_TEST);

	renderObjects();

	//renderLight();

	glBindFramebuffer(GL_FRAMEBUFFER, screenRectFBO);
	Shader shaderTexture = Shaders[SHADER_TEXTURE];
	shaderTexture.use();

	glUniform1i(glGetUniformLocation(shaderTexture.getId(), "screenTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);

	glBindVertexArray(screenRectVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

GLuint DeferredRenderer::getTexture() {
	return screenRectTexture;
}

void DeferredRenderer::resize(int width, int height) {
	/*
	Deferred Rendering Textures
	*/
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	glBindTexture(GL_TEXTURE_2D, gBufferPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gBufferPosition, 0);

	glBindTexture(GL_TEXTURE_2D, gBufferNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gBufferNormal, 0);

	glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gBufferAlbedo, 0);

	glBindTexture(GL_TEXTURE_2D, gBufferOption1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gBufferOption1, 0);

	glBindTexture(GL_TEXTURE_2D, gBloom);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gBloom, 0);

	GLuint gAttachments[5] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
	glDrawBuffers(5, gAttachments);

	glBindRenderbuffer(GL_RENDERBUFFER, rboGDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	// - Attach buffers

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboGDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
	glBindTexture(GL_TEXTURE_2D, bloomTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomTexture, 0);
	GLuint attachments[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	glBindTexture(GL_TEXTURE_2D, lightTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightTexture, 0);
	GLuint lightAttachments[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, lightAttachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	/*
	Screen Texture
	*/
	glBindFramebuffer(GL_FRAMEBUFFER, screenRectFBO);
	glBindTexture(GL_TEXTURE_2D, screenRectTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, getWidth(), getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenRectTexture, 0);
	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void DeferredRenderer::renderObjects()
{
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
	glViewport(0, 0, getWidth(), getHeight());

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glStencilMask(0x00);

	//bind global camera matrices and properties
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera->getViewMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->getCameraMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(GLfloat), &camera->FarZ);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//draw 3d pointlight object
	shader_light.use();
	for (auto plight : pointLights)
	{
		glUniform1f(glGetUniformLocation(shader_light.getId(), "intensity"), plight->intensity);
		glUniform3f(glGetUniformLocation(shader_light.getId(), "color"), plight->diffuse.r, plight->diffuse.g, plight->diffuse.b);
		plight->draw(shader_light);
	}

	for (auto instancer : instancers)
	{
		instancer->draw();
	}

	//draw particles

	//draw all drawables
	for (auto drawable : objects)
	{
		drawable->draw();
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
	shader_normals.use();
	for (auto drawable : objects)
	{
		if (drawable->settings.normalVisible)
		{
			drawable->drawNormals(shader_normals);
		}
	}
	//draw bounding box of drawable (not aabb)
	for (auto drawable : objects)
	{
		if (drawable->settings.boxVisible)
		{
			drawable->drawBox();
		}
	}

	//draw aabb from drawables
	shader_geometry.use();
	glUniform4f(glGetUniformLocation(shader_geometry.getId(), "color"), 0.0f, 1.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader_geometry.getId(), "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

	glDisable(GL_STENCIL_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void DeferredRenderer::bloom()
{
	Shader blur = Shaders[SHADER_FILTER_BLUR];

	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	blur.use();

	glUniform1i(glGetUniformLocation(blur.getId(), "targetTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBloom);

	glBindVertexArray(screenRectVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::renderLight()
{
	/*
	//set viewfrustum fro directional light (fro cascaded shadow mapping)
	if (directionalLight)
	{
		directionalLight->setViewFrustum(
			camera->getViewFrustum(directionalLight->getCSMSlices()));
	}

	//render csm directionLight
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (directionalLight)
	{
		Shader shader_depth = Shaders[SHADER_DEPTH];
		for (int i = 0; i < directionalLight->getCSMSlices(); i++)
		{
			directionalLight->begin_shadow_mapping(i);
			for (auto drawable : objects)
			{
				glUniformMatrix4fv(glGetUniformLocation(shader_depth.getId(), "model"), 1, GL_FALSE, glm::value_ptr(drawable->getModelMatrix()));
				drawable->draw(shader_depth);
			}
			directionalLight->end_shadow_mapping();
		}
	}

	//Point Light Shader is deactivated

	//if (point_lights.size() > 0)
	//{
	//	Shader shader_depth_cube = Shaders[SHADER_DEPTH_CUBE];
	//	for (auto plight : point_lights)
	//	{
	//		plight->begin_shadow_mapping();
	//		for (auto drawable : objects)
	//		{
	//			glUniformMatrix4fv(glGetUniformLocation(shader_depth_cube.getId(), "model"), 1, GL_FALSE, glm::value_ptr(drawable->getModelMatrix()));
	//			drawable->draw(shader_depth_cube);
	//		}
	//		plight->end_shadow_mapping();
	//	}
	//}
	Shader shader_deferred;
	glm::vec3 viewPos = camera->getPosition();

	Shader shader_deferred = Shaders[SHADER_DEFERRED];

	glBindFramebuffer(GL_FRAMEBUFFER, screenRectFBO);
	shader_deferred.use();
	glDisable(GL_DEPTH_TEST);

	if (dConfig.visibleDirectionalLight)
	{
		glUniform3f(glGetUniformLocation(shader_deferred.getId(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);

		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gPosition"), 0);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gNormal"), 1);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gAlbedo"), 2);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gOption1"), 3);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gBloom"), 4);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBufferPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gBufferNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gBufferOption1);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, bloomTexture);

		if (directionalLight)
		{
			directionalLight->apply(shader_deferred, "dirLight");
			for (int i = 0; i < directionalLight->getCSMSlices(); i++)
			{
				glUniform1i(glGetUniformLocation(shader_deferred.getId(), ("dirLight.shadowMap[" + std::to_string(i) + "]").c_str()), 5 + i);
				glActiveTexture(GL_TEXTURE5 + i);
				glBindTexture(GL_TEXTURE_2D, directionalLight->getShadowMap(i));
			}
		}

		//set back to fill mode for texture rendering
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	if (dConfig.visiblePointLight)
	{

		shader_deferred = Shaders[SHADER_DEFFERED_PLIGHT_NOS];
		shader_deferred.use();
		glUniform3f(glGetUniformLocation(shader_deferred.getId(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);

		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gPosition"), 0);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gNormal"), 1);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gAlbedo"), 2);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gOption1"), 3);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "prevTexture"), 4);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBufferPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gBufferNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gBufferOption1);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, sceneCamera.getTexture());

		///
		/// For simplicity point and spotlight shadows have been disabled
		///

		GLint plight_count = 0;
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "pointLights"), pointLights.size());
		for (auto plight : pointLights)
		{
			plight->apply(shader_deferred, "pointLight[" + std::to_string(plight_count) + "]");
			plight_count++;
		}

		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		sceneCamera.endDrawing();
	}

	//begin spot light rendering
	if (dConfig.visibleSpotLight)
	{
		shader_deferred = Shaders[SHADER_DEFFERED_SLIGHT_NOS];
		sceneCamera.beginDrawing(shader_deferred);
		glUniform3f(glGetUniformLocation(shader_deferred.getId(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);

		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gPosition"), 0);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gNormal"), 1);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gAlbedo"), 2);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gOption1"), 3);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "prevTexture"), 4);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBufferPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gBufferNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gBufferOption1);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, sceneCamera.getTexture());

		GLuint slightCount = 0;
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "spotLights"), spotLights.size());
		for (auto sLight : spotLights)
		{
			sLight->apply(shader_deferred, "spotLight[" + std::to_string(slightCount) + "]");
		}

		//merge sportlight scene + old scene
		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		sceneCamera.endDrawing();
	}

	//render just on camera textrue if no lights active
	if (!sceneCamera.config.dLightVisible && !sceneCamera.config.pLightVisible && !sceneCamera.config.sLightVisible)
	{

		shader_deferred = Shaders[SHADER_TEXTURE];
		sceneCamera.beginDrawing(shader_deferred);

		glUniform1i(glGetUniformLocation(Shaders[SHADER_TEXTURE].getId(), "screenTexture"), 0);
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);

		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		sceneCamera.endDrawing();
	}
	*/
}

void DeferredRenderer::setup() {
	renderMode = RenderMode::FILLR;

	glGenFramebuffers(1, &gBufferFBO);
	glGenFramebuffers(1, &bloomFBO);
	glGenFramebuffers(1, &lightFBO);

	glGenTextures(1, &gBufferPosition);
	glGenTextures(1, &gBufferNormal);
	glGenTextures(1, &gBufferAlbedo);
	glGenTextures(1, &gBufferOption1);
	glGenTextures(1, &gBloom);
	glGenTextures(1, &bloomTexture);
	glGenTextures(1, &screenTexture);

	glGenRenderbuffers(1, &rboGDepth);

	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, getRendererId(), uboMatrices, 0, 2 * sizeof(glm::mat4) + sizeof(GLfloat));

	GLfloat vertices_rect[] = {
		-1.0f,
		1.0f,
		0.0f,
		0.0f,
		1.0f,
		-1.0f,
		-1.0f,
		0.0f,
		0.0f,
		0.0f,
		1.0f,
		1.0f,
		0.0f,
		1.0f,
		1.0f,
		1.0f,
		-1.0f,
		0.0f,
		1.0f,
		0.0f,
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

	shader_basic = Shaders[SHADER_BASIC];
	shader_light = Shaders[SHADER_DEFFERED_LIGHT];
	shader_normals = Shaders[SHADER_DEFFERED_NORMALS];
	shader_geometry = Shaders[SHADER_DEFFERED_GEOMETRY];
}