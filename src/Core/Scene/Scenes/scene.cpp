#include "scene.h"


Scene::Scene(int width, int height) : AbstractScene(width, height)
{
	setup();
	resize(width, height);
}


void Scene::tick(float delta)
{
	int width = getWidth();
	int height = getHeight();

	//glDisable(GL_CULL_FACE);
	//Point and Spotlights not supported

	//if (spotLights.size() > 0) {
	//	Shader shader_depth = Shaders[SHADER_DEPTH_PERSP];
	//	for (auto sLight : spotLights) {
	//		sLight->beginShadowMapping();
	//		for (auto drawable : objects)
	//		{
	//			glUniformMatrix4fv(glGetUniformLocation(shader_depth.getId(), "model"), 1, GL_FALSE, glm::value_ptr(drawable->getModelMatrix()));
	//			drawable->draw(shader_depth);
	//		}
	//		sLight->endShadadowMapping();
	//	}
	//}

	glUniformBlockBinding(Shaders[SHADER_BASIC].getId(), glGetUniformBlockIndex(Shaders[SHADER_BASIC].getId(), "Matrices"), getSceneId());
	glUniformBlockBinding(Shaders[SHADER_FONT].getId(), glGetUniformBlockIndex(Shaders[SHADER_FONT].getId(), "Matrices"), getSceneId());
	glUniformBlockBinding(Shaders[SHADER_SKYBOX].getId(), glGetUniformBlockIndex(Shaders[SHADER_SKYBOX].getId(), "Matrices"), getSceneId());
	glUniformBlockBinding(Shaders[SHADER_DEFFERED_LIGHT].getId(), glGetUniformBlockIndex(Shaders[SHADER_DEFFERED_LIGHT].getId(), "Matrices"), getSceneId());
	glUniformBlockBinding(Shaders[SHADER_DEFFERED_GEOMETRY].getId(), glGetUniformBlockIndex(Shaders[SHADER_DEFFERED_GEOMETRY].getId(), "Matrices"), getSceneId());
	glUniformBlockBinding(Shaders[SHADER_INSTANCING_BASIC].getId(), glGetUniformBlockIndex(Shaders[SHADER_INSTANCING_BASIC].getId(), "Matrices"), getSceneId());


	//update animatables
	for (auto animatable : animatables) {
		animatable->update(delta);
	}

	int polygonMode = GL_FILL;
	if (renderMode == RenderMode::POINTR)
		polygonMode = GL_POINT;
	else if (renderMode == RenderMode::LINER)
		polygonMode = GL_LINE;

	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);


	for (auto &sceneCamera : cameras) {
		std::sort(objects.begin(), objects.end(), SortDrawable(sceneCamera.camera->getPosition()));



		//only draw active cameras
		if (!sceneCamera.config.Active) {
			continue;
		}

		if (sceneCamera.config.dLightVisible) {

			//set viewfrustum fro directional light (fro cascaded shadow mapping)
			if (directionalLight) {
				directionalLight->setViewFrustum(
					sceneCamera.camera->getViewFrustum(directionalLight->getCSMSlices())
				);
			}

			//render csm directionLight
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			if (directionalLight)
			{
				Shader shader_depth = Shaders[SHADER_DEPTH];
				for (int i = 0; i < directionalLight->getCSMSlices(); i++) {
					directionalLight->begin_shadow_mapping(i);
					for (auto drawable : objects)
					{
						glUniformMatrix4fv(glGetUniformLocation(shader_depth.getId(), "model"), 1, GL_FALSE, glm::value_ptr(drawable->getModelMatrix()));
						drawable->draw(shader_depth);
					}
					directionalLight->end_shadow_mapping();
				}
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


		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
		glViewport(0, 0, width, height);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glStencilMask(0x00);

		//bind global camera matrices and properties
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(sceneCamera.camera->getViewMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(sceneCamera.camera->getCameraMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(GLfloat), &sceneCamera.camera->FarZ);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


		//draw 3d pointlight object
		shader_light.use();
		for (auto plight : pointLights)
		{
			glUniform1f(glGetUniformLocation(shader_light.getId(), "intensity"), plight->intensity);
			glUniform3f(glGetUniformLocation(shader_light.getId(), "color"), plight->diffuse.r, plight->diffuse.g, plight->diffuse.b);
			plight->draw(shader_light);
		}

		for (auto instancer : instancers) {
			instancer->draw();
		}

		//draw particles

		//draw all drawables
		for (auto drawable : objects)
		{
			drawable->draw();
		}

		//draw viewfrustums from cameras
		for (auto &scam : cameras) {
			if (&scam != &sceneCamera && scam.config.debugVisible) {
				Geometry geoCam = scam.getDebugViewFrustum(directionalLight->getCSMSlices());
				geoCam.draw();
				geoCam.dispose();
			}
		}

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
		for (auto drawable : objects) {
			if (drawable->settings.boxVisible) {
				drawable->drawBox();
			}
		}


		//draw aabb from drawables
		shader_geometry.use();
		glUniform4f(glGetUniformLocation(shader_geometry.getId(), "color"), 0.0f, 1.0f, 0.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader_geometry.getId(), "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

		glDisable(GL_STENCIL_TEST);

		//bloom effect
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


		//
		// begin drawing to current camera
		//

		Shader shader_deferred = Shaders[SHADER_DEFERRED];
		glViewport(0, 0, width, height);
		sceneCamera.clear();
		glm::vec3 viewPos = glm::vec3(sceneCamera.camera->getPosition().x, sceneCamera.camera->getPosition().y, sceneCamera.camera->getPosition().z);

		if (sceneCamera.config.dLightVisible) {
			sceneCamera.beginDrawing(shader_deferred);


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
				for (int i = 0; i < directionalLight->getCSMSlices(); i++) {
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

			sceneCamera.endDrawing();
		}

		if (sceneCamera.config.pLightVisible) {

			shader_deferred = Shaders[SHADER_DEFFERED_PLIGHT_NOS];
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
		if (sceneCamera.config.sLightVisible) {
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
			for (auto sLight : spotLights) {
				sLight->apply(shader_deferred, "spotLight[" + std::to_string(slightCount) + "]");
			}

			//merge sportlight scene + old scene
			glBindVertexArray(screenRectVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);

			sceneCamera.endDrawing();
		}

		//render just on camera textrue if no lights active
		if (!sceneCamera.config.dLightVisible && !sceneCamera.config.pLightVisible && !sceneCamera.config.sLightVisible) {

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

	}
	//glBindBuffer(GL_FRAMEBUFFER, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	Shaders[SHADER_TEXTURE].use();
	glUniform1i(glGetUniformLocation(Shaders[SHADER_TEXTURE].getId(), "screenTexture"), 0);
	glActiveTexture(GL_TEXTURE0);

	for (auto sceneCamera : cameras) {
		//only draw active cameras
		if (!sceneCamera.config.Active) {
			continue;
		}
		glBindTexture(GL_TEXTURE_2D, sceneCamera.getTexture());
		//glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);

		glBindVertexArray(sceneCamera.getFrameVerticesVAO());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
	glEnable(GL_DEPTH_TEST);
}

void Scene::dispose()
{
	while (objects.size() > 0)
	{
		objects[objects.size() - 1]->dispose();
		objects.pop_back();
	}


	for (auto camera : cameras) {
		camera.dispose();
	}


	glDeleteFramebuffers(1, &gBufferFBO);
	glDeleteFramebuffers(1, &bloomFBO);
	glDeleteFramebuffers(1, &lightFBO);

	glDeleteRenderbuffers(1, &rboGDepth);
	glDeleteTextures(1, &gBufferAlbedo);
	glDeleteTextures(1, &gBufferNormal);
	glDeleteTextures(1, &gBufferPosition);
	glDeleteTextures(1, &gBufferOption1);
	glDeleteTextures(1, &gBloom);

	glDeleteTextures(1, &lightTexture);

	glDeleteBuffers(1, &uboMatrices);

	glDeleteBuffers(1, &screenRectVBO);
	glDeleteVertexArrays(1, &screenRectVAO);


	glDeleteBuffers(1, &screenShadowVBO);
	glDeleteVertexArrays(1, &screenShadowVAO);

}

void Scene::resize(int width, int height)
{
	AbstractScene::resize(width, height);

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

	GLuint gAttachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
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
	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
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
	GLuint lightAttachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, lightAttachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::addObject(SceneObject & object)
{
	if (Drawable * drawable = dynamic_cast<Drawable*>(&object)) {
		objects.push_back(drawable);
		return;
	}

	if (DirectionalLight * dLight = dynamic_cast<DirectionalLight*>(&object)) {
		directionalLight = dLight;
		return;
	}

	if (Animatable * animatable = dynamic_cast<Animatable*>(&object)) {
		animatables.push_back(animatable);
		return;
	}

	if (Instancer * instancer = dynamic_cast<Instancer*>(instancer)) {
		instancers.push_back(instancer);
		return;
	}

	if (SpotLight * sLight = dynamic_cast<SpotLight*>(&object)) {
		spotLights.push_back(sLight);
		return;
	}
	
	if (PointLight * pLight = dynamic_cast<PointLight*>(&object)) {
		pointLights.push_back(pLight);
		return;
	}

	printf("[Engine] [Scene] [Error] Scene doesn't accept %s\n", object.getId());
}

void Scene::addObject(Camera &camera, Size size)
{
	cameras.push_back(SceneCamera(camera, size, getWidth(), getHeight()));
}

void Scene::removeObject(SceneObject &object)
{
	if (Drawable * drawable = dynamic_cast<Drawable*>(&object)) {
		for (size_t i = 0; i < objects.size(); i++) {
			if (objects[i] == drawable) {
				objects.erase(objects.begin() + i);
				i--;
			}
		}
		return;
	}

	if (DirectionalLight * dLight = dynamic_cast<DirectionalLight*>(&object)) {
		directionalLight = dLight;
		return;
	}

	if (Animatable * animatable = dynamic_cast<Animatable*>(&object)) {
		for (size_t i = 0; i < animatables.size(); i++) {
			if (animatables[i] == animatable) {
				animatables.erase(animatables.begin() + i);
				i--;
			}
		}
		return;
	}

	if (Instancer * instancer = dynamic_cast<Instancer*>(&object)) {
		instancers.push_back(instancer);
		return;
	}

	if (SpotLight * sLight = dynamic_cast<SpotLight*>(&object)) {
		spotLights.push_back(sLight);
		return;
	}

	if (PointLight * pLight = dynamic_cast<PointLight*>(&object)) {
		pointLights.push_back(pLight);
		return;
	}

	if (Camera * camera = dynamic_cast<Camera*>(&object)) {
		for (size_t i = 0; i < cameras.size(); i++) {
			if (cameras[i].camera == camera) {
				cameras[i].dispose();
				cameras.erase(cameras.begin() + i);
				i--;
			}
		}
	}

	printf("[Engine] [Scene] [Error] Scene doesn't accept %s\n", object.getId());
}

SceneCameraConfig Scene::getCameraConfig(Camera & camera)
{
	for (size_t i = 0; i < cameras.size(); i++) {
		if (cameras[i].camera == &camera) {
			return cameras[i].config;
		}
	}
	throw std::invalid_argument("Camera not found");
}

void Scene::enableCamera(Camera & camera, bool enable)
{
	for (size_t i = 0; i < cameras.size(); i++) {
		if (cameras[i].camera == &camera) {
			cameras[i].config.Active = enable;
		}
	}
}

void Scene::configureCamera(Camera & camera, SceneCameraConfig config)
{
	for (size_t i = 0; i < cameras.size(); i++) {
		if (cameras[i].camera == &camera) {
			cameras[i].config = config;
		}
	}
}

void Scene::setup()
{
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
	glGenTextures(1, &lightTexture);

	glGenRenderbuffers(1, &rboGDepth);

	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, getSceneId(), uboMatrices, 0, 2 * sizeof(glm::mat4) + sizeof(GLfloat));


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


	shader_basic = Shaders[SHADER_BASIC];
	shader_light = Shaders[SHADER_DEFFERED_LIGHT];
	shader_normals = Shaders[SHADER_DEFFERED_NORMALS];
	shader_geometry = Shaders[SHADER_DEFFERED_GEOMETRY];
}
