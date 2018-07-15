#include "scene.h"

Scene::Scene()
{
	setup();
	reload(width, height);
}

Scene::Scene(int width, int height)
{
	setup();
	reload(width, height);
}

Scene::~Scene()
{
}

void Scene::addDrawable(Drawable &drawable)
{
	objects.push_back(&drawable);
}

void Scene::addRigidBody(RigidBody & rigidBody)
{
	rigidBodys.push_back(&rigidBody);
	dynamicsWorld->addRigidBody(rigidBody.getBody());
}

void Scene::addVehicle(Vehicle & vehicle)
{
	vehicles.push_back(&vehicle);
	dynamicsWorld->addVehicle(vehicle.getVehicle());
}

void Scene::addParticleGenerator(ParticleGenerator &pg)
{
	particleGenerators.push_back(&pg);
}

void Scene::addPlight(PointLight &plight)
{
	pointLights.push_back(&plight);
}

void Scene::addSLight(SpotLight & sLight)
{
	spotLights.push_back(&sLight);
}


void Scene::removeDrawable(Drawable & drawable)
{
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i] == &drawable) {
			objects.erase(objects.begin() + i);
			i--;
		}
	}
}

void Scene::removeRigidBody(RigidBody & rigidBody)
{
	dynamicsWorld->removeRigidBody(rigidBody.getBody());
}

void Scene::removeVehicle(Vehicle & vehicle)
{
	for (int i = 0; i < vehicles.size(); i++) {
		if (vehicles[i] == &vehicle) {
			dynamicsWorld->removeVehicle(vehicle.getVehicle());
			vehicles.erase(vehicles.begin() + i);
			i--;
		}
	}
}

void Scene::removeParticleGenerator(ParticleGenerator & particleGenerator)
{
	for (int i = 0; i < particleGenerators.size(); i++) {
		if (particleGenerators[i] == &particleGenerator) {
			particleGenerators.erase(particleGenerators.begin() + i);
			i--;
		}
	}
}

void Scene::removeCamera(Camera & camera)
{
	for (int i = 0; i < cameras.size(); i++) {
		if (cameras[i].camera == &camera) {
			cameras.erase(cameras.begin() + i);
			i--;
		}
	}
}

void Scene::enableCamera(Camera & camera, bool enable)
{
	for (int i = 0; i < cameras.size(); i++) {
		if (cameras[i].camera == &camera) {
			cameras[i].active = enable;
		}
	}
}

void Scene::addCamera(Camera & camera, Size size)
{
	cameras.push_back(SceneCamera(camera, size));
}

void Scene::setDlight(DirectionalLight &dlight)
{
	directionalLight = &dlight;
}

btDiscreteDynamicsWorld* Scene::getPhysicsWorld() {
	return dynamicsWorld;
}

void Scene::draw(GLfloat delta)
{
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


	for (auto sceneCamera : cameras) {
		//only draw active cameras
		if (!sceneCamera.active) {
			continue;
		}

		if (directionalLight) {
			directionalLight->setViewFrustum(
				sceneCamera.camera->getViewFrustum(directionalLight->getCSMSlices())
			);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		


		glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
		glViewport(0, 0, width, height);
		glClearColor(0.001f, 0.001f, 0.001f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(sceneCamera.camera->getViewMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(sceneCamera.camera->getCameraMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(GLfloat), &sceneCamera.camera->FarZ);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);



		shader_light.use();
		for (auto plight : pointLights)
		{
			glUniform1f(glGetUniformLocation(shader_light.getId(), "intensity"), plight->intensity);
			glUniform3f(glGetUniformLocation(shader_light.getId(), "color"), plight->diffuse.r, plight->diffuse.g, plight->diffuse.b);
			plight->draw(shader_light);
		}

		for (auto pg : particleGenerators) {
			pg->update(delta);
			pg->draw(delta, shader_basic);
		}

		for (auto drawable : objects)
		{
			drawable->draw();
		}

		shader_normals.use();
		for (auto drawable : objects)
		{
			if (drawable->visibleNormal)
			{
				drawable->drawNormals(shader_normals);
			}
		}
		for (auto drawable : objects) {
			if (drawable->visibleBox) {
				drawable->drawBox();
			}
		}

		shader_geometry.use();
		glUniform4f(glGetUniformLocation(shader_geometry.getId(), "color"), 0.0f, 1.0f, 0.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader_geometry.getId(), "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		for (auto body : rigidBodys) {
			if (body->visibleAABB)
				body->drawAABB(shader_geometry);
		}

		//
		// begin drawing to current camera
		//

		Shader shader_deferred = Shaders[SHADER_DEFERRED];
		sceneCamera.beginDrawing(shader_deferred);
		glViewport(0, 0, width, height);
		glm::vec3 viewPos = glm::vec3(sceneCamera.camera->getPosition().x, sceneCamera.camera->getPosition().y, sceneCamera.camera->getPosition().z);
		glUniform3f(glGetUniformLocation(shader_deferred.getId(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);

		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gPosition"), 0);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gNormal"), 1);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gAlbedo"), 2);
		glUniform1i(glGetUniformLocation(shader_deferred.getId(), "gUseLight"), 3);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBufferPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gBufferNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gBufferUseLight);

		if (directionalLight)
		{
			directionalLight->apply(shader_deferred, "dirLight");
			for (int i = 0; i < directionalLight->getCSMSlices(); i++) {
				glUniform1i(glGetUniformLocation(shader_deferred.getId(), ("dirLight.shadowMap[" + std::to_string(i) + "]").c_str()), 4 + i);
				glActiveTexture(GL_TEXTURE4 + i);
				glBindTexture(GL_TEXTURE_2D, directionalLight->getShadowMap(i));
			}
		}


		///
		/// For simplicity point and spotlights have been disabled
		/// 
		
		//GLint plight_count = 0;
		//glUniform1i(glGetUniformLocation(shader_deferred.getId(), "pointLights"), pointLights.size());
		//for (auto plight : pointLights)
		//{
		//	plight->apply(shader_deferred, "pointLight[" + std::to_string(plight_count) + "]");
		//	glUniform1i(glGetUniformLocation(shader_deferred.getId(), ("pointLight[" + std::to_string(plight_count) + "].shadowCubeMap").c_str()), 8 + plight_count);
		//	glActiveTexture(GL_TEXTURE8 + plight_count);
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, plight->get_shadow_cube_map());
		//	plight_count++;
		//}

		//Point Light Shadow is deactivated

		//
		//for (int i = plight_count; i < 5; i++)
		//{
		//	PointLight *plight = point_lights[plight_count - 1];
		//	plight->apply(shader_deferred, "pointLight[" + std::to_string(i) + "]");
		//	glUniform1i(glGetUniformLocation(shader_deferred.getId(), ("pointLight[" + std::to_string(i) + "].shadowCubeMap").c_str()), 5 + i);
		//	glActiveTexture(GL_TEXTURE5 + (plight_count - 1));
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, plight->get_shadow_cube_map());
		//}
		//


		//for (auto sLight : spotLights) {
		//	sLight->apply(shader_deferred, "spotLight");
		//	glUniform1i(glGetUniformLocation(shader_deferred.getId(), "spotLight.shadowMap"), 10);
		//	glActiveTexture(GL_TEXTURE10);
		//	glBindTexture(GL_TEXTURE_2D, sLight->getShadowMap());
		//}
		

		glBindVertexArray(screenRectVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		sceneCamera.endDrawing();
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	Shaders[SHADER_TEXTURE].use();
	glUniform1i(glGetUniformLocation(Shaders[SHADER_TEXTURE].getId(), "screenTexture"), 0);
	glActiveTexture(GL_TEXTURE0);

	for (auto sceneCamera : cameras) {
		//only draw active cameras
		if (!sceneCamera.active) {
			continue;
		}
		glBindTexture(GL_TEXTURE_2D, sceneCamera.getTexture());

		glBindVertexArray(sceneCamera.getFrameVerticesVAO());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
	glEnable(GL_DEPTH_TEST);
	
}

void Scene::updatePhysics(GLfloat delta)
{
	//Physics
	dynamicsWorld->stepSimulation(delta, 10);

	for (auto body : rigidBodys) {
		body->syncDrawable();
	}

	for (auto vehicle : vehicles) {
		vehicle->sync();
	}
}

void Scene::dispose()
{
	while (objects.size() > 0)
	{
		objects[objects.size() - 1]->dispose();
		objects.pop_back();
	}


	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[i];
		dynamicsWorld->removeCollisionObject(obj);
	}
	rigidBodys.clear();

	delete dynamicsWorld;
	delete solver;	
	delete dispatcher;
	delete collisionConfiguration;
	delete overlappingPairCache;


	for (auto camera : cameras) {
		camera.dispose();
	}


	glDeleteFramebuffers(1, &gBufferFBO);
	glDeleteRenderbuffers(1, &rboGDepth);
	glDeleteTextures(1, &gBufferAlbedo);
	glDeleteTextures(1, &gBufferNormal);
	glDeleteTextures(1, &gBufferPosition);
	glDeleteTextures(1, &gBufferUseLight);
	glDeleteBuffers(1, &uboMatrices);

	glDeleteBuffers(1, &screenRectVBO);
	glDeleteVertexArrays(1, &screenRectVAO);


	glDeleteBuffers(1, &screenShadowVBO);
	glDeleteVertexArrays(1, &screenShadowVAO);
	
}

void Scene::reload(int width, int height)
{
	this->width = width;
	this->height = height;

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

	glBindTexture(GL_TEXTURE_2D, gBufferUseLight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gBufferUseLight, 0);

	GLuint gAttachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, gAttachments);

	glBindRenderbuffer(GL_RENDERBUFFER, rboGDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	// - Attach buffers

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboGDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::setup()
{
	glGenFramebuffers(1, &gBufferFBO);
	glGenTextures(1, &gBufferPosition);
	glGenTextures(1, &gBufferNormal);
	glGenTextures(1, &gBufferAlbedo);
	glGenTextures(1, &gBufferUseLight);

	glGenRenderbuffers(1, &rboGDepth);

	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4) + sizeof(GLfloat));


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

	

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

	shader_basic = Shaders[SHADER_BASIC];
	shader_light = Shaders[SHADER_DEFFERED_LIGHT];
	shader_normals = Shaders[SHADER_DEFFERED_NORMALS];
	shader_geometry = Shaders[SHADER_DEFFERED_GEOMETRY];
}
