#include "scene.h"

Scene::Scene()
{
	setup(800, 600);
	gravity = rp3d::Vector3(0.0, -1.81, 0.0);
	world = new rp3d::DynamicsWorld(gravity);
	world->setNbIterationsVelocitySolver(15);
	world->setNbIterationsPositionSolver(8);
	world->enableSleeping(true);
}

Scene::~Scene()
{
	for (auto drawable : objects) {
		world->destroyRigidBody(drawable->body);
		drawable->body = nullptr;
	}
	delete world;
}

void Scene::add_object(Drawable &drawable)
{
	rp3d::Vector3 position(drawable.get_position().x, drawable.get_position().y, drawable.get_position().z);
	rp3d::Quaternion rotation = rp3d::Quaternion(drawable.get_rotation().x, drawable.get_rotation().y, drawable.get_rotation().z, 1.0f);
	rp3d::Transform transform(position, rotation);
	drawable.body = world->createRigidBody(transform);
	drawable.body->setType(rp3d::BodyType::STATIC);
	drawable.body->enableGravity(false);

	rp3d::Material &material = drawable.body->getMaterial();
	material.setBounciness(rp3d::decimal(0.1f));
	material.setFrictionCoefficient(rp3d::decimal(1.0f)); 

	rp3d::Vector3 box_size(drawable.get_size().width * 0.5f, drawable.get_size().height * 0.5, drawable.get_size().depth * 0.0);
	drawable.box_shape =  new rp3d::BoxShape(box_size);
	


	drawable.proxy_shape = drawable.body->addCollisionShape(drawable.box_shape, transform, 0.4f);
	
	objects.push_back(&drawable);
}

void Scene::add_plight(PointLight &plight)
{
	point_lights.push_back(&plight);
}

void Scene::set_camera(Camera &camera)
{
	this->camera = &camera;
}

void Scene::set_dlight(DirectionalLight &dlight)
{
	directional_light = &dlight;
}

void Scene::draw(GLfloat delta)
{
	//calculate physics
	rp3d_accumulator += delta;
	while (rp3d_accumulator >= rp3d_time_step)
	{
		world->update(rp3d_time_step);
		rp3d_accumulator -= rp3d_time_step;
	}

	rp3d::decimal rp3d_factor = rp3d_accumulator / rp3d_time_step;

	for (auto drawable : objects) {
		rp3d::Transform rp3d_current_transform = drawable->body->getTransform();
		rp3d::Transform rp3d_interp_transform = rp3d::Transform::interpolateTransforms(drawable->rp3d_prev_transform, rp3d_current_transform, rp3d_factor);
		drawable->rp3d_prev_transform = rp3d_current_transform;
		
		rp3d::Vector3 rotation = rp3d_current_transform.getOrientation().getVectorV();
		rp3d::Vector3 position = rp3d_current_transform.getPosition();

		drawable->set_position(glm::vec3(position.x, position.y, position.z));
		
		drawable->rotate(glm::vec3(rotation.x, rotation.y, rotation.z));
	}


	Shader shader_basic = Shaders[SHADER_BASIC];
	Shader shader_light = Shaders[SHADER_DEFFERED_LIGHT];
	Shader shader_normals = Shaders[SHADER_DEFFERED_NORMALS];
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (directional_light)
	{
		Shader shader_depth = Shaders[SHADER_DEPTH];

		directional_light->begin_shadow_mapping();
		for (auto drawable : objects)
		{
			glUniformMatrix4fv(glGetUniformLocation(shader_depth.get_id(), "model"), 1, GL_FALSE, glm::value_ptr(drawable->get_model_matrix()));
			drawable->draw(shader_depth);
		}
		directional_light->end_shadow_mapping();
	}	
	//Point Light Shader is deactivated
	/*
	if (point_lights.size() > 0)
	{
		Shader shader_depth_cube = Shaders[SHADER_DEPTH_CUBE];
		for (auto plight : point_lights)
		{
			plight->begin_shadow_mapping();
			for (auto drawable : objects)
			{
				glUniformMatrix4fv(glGetUniformLocation(shader_depth_cube.get_id(), "model"), 1, GL_FALSE, glm::value_ptr(drawable->get_model_matrix()));
				drawable->draw(shader_depth_cube);
			}
			plight->end_shadow_mapping();
		}
	}
	*/

	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	glViewport(0, 0, width, height);
	glClearColor(0.001f, 0.001f, 0.001f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera->GetViewMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->GetCameraMatrix(width, height)));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//glDisable(GL_CULL_FACE);


	shader_light.use();
	for (auto plight : point_lights)
	{
		glUniform1f(glGetUniformLocation(shader_light.get_id(), "intensity"), plight->intensity);
		glUniform3f(glGetUniformLocation(shader_light.get_id(), "color"), plight->diffuse.r, plight->diffuse.g, plight->diffuse.b);
		plight->draw(shader_light);
	}
	
	for (auto drawable : objects)
	{
		drawable->draw();
	}
	shader_normals.use();
	for (auto drawable : objects)
	{
		if (drawable->visible_normal)
		{
			drawable->draw_normals(shader_normals);
		}
	}
	for (auto drawable : objects) {	
		if (drawable->visible_box) {
			drawable->draw_box();
		}
	}

	//render to window frame
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader shader_deferred = Shaders[SHADER_DEFERRED];
	shader_deferred.use();

	glUniform3f(glGetUniformLocation(shader_deferred.get_id(), "viewPos"), camera->get_position().x, camera->get_position().y, camera->get_position().z);

	glUniform1i(glGetUniformLocation(Shaders[SHADER_DEFERRED].get_id(), "gPosition"), 0);
	glUniform1i(glGetUniformLocation(Shaders[SHADER_DEFERRED].get_id(), "gNormal"), 1);
	glUniform1i(glGetUniformLocation(Shaders[SHADER_DEFERRED].get_id(), "gAlbedo"), 2);
	glUniform1i(glGetUniformLocation(Shaders[SHADER_DEFERRED].get_id(), "gUseLight"), 3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBufferPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBufferNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gBufferUseLight);

	if (directional_light)
	{
		directional_light->apply(shader_deferred, "dirLight");
		glUniform1i(glGetUniformLocation(shader_deferred.get_id(), "dirLight.shadowMap"), 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, directional_light->get_shadow_map());
	}
	GLint plight_count = 0;
	glUniform1i(glGetUniformLocation(shader_deferred.get_id(), "pointLights"), point_lights.size());
	for (auto plight : point_lights)
	{
		plight->apply(shader_deferred, "pointLight[" + std::to_string(plight_count) + "]");
		glUniform1i(glGetUniformLocation(shader_deferred.get_id(), ("pointLight[" + std::to_string(plight_count) + "].shadowCubeMap").c_str()), 5 + plight_count);
		glActiveTexture(GL_TEXTURE5 + plight_count);
		glBindTexture(GL_TEXTURE_CUBE_MAP, plight->get_shadow_cube_map());
		plight_count++;
	}
	//Point Light Shadow is deactivated
	/*
	for (int i = plight_count; i < 5; i++)
	{
		PointLight *plight = point_lights[plight_count - 1];
		plight->apply(shader_deferred, "pointLight[" + std::to_string(i) + "]");
		glUniform1i(glGetUniformLocation(shader_deferred.get_id(), ("pointLight[" + std::to_string(i) + "].shadowCubeMap").c_str()), 5 + i);
		glActiveTexture(GL_TEXTURE5 + (plight_count - 1));
		glBindTexture(GL_TEXTURE_CUBE_MAP, plight->get_shadow_cube_map());
	}
	*/
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

	GLuint screenRectVBO;
	GLuint screenRectVAO;

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

	glBindVertexArray(screenRectVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Scene::dispose()
{
	while (objects.size() > 0)
	{
		objects.pop_back();
	}
}

void Scene::setup(int width, int height)
{
	this->width = width;
	this->height = height;
	glGenFramebuffers(1, &gBufferFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, gBufferFBO);
	glGenTextures(1, &gBufferPosition);
	glBindTexture(GL_TEXTURE_2D, gBufferPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gBufferPosition, 0);

	glGenTextures(1, &gBufferNormal);
	glBindTexture(GL_TEXTURE_2D, gBufferNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gBufferNormal, 0);

	glGenTextures(1, &gBufferAlbedo);
	glBindTexture(GL_TEXTURE_2D, gBufferAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gBufferAlbedo, 0);

	glGenTextures(1, &gBufferUseLight);
	glBindTexture(GL_TEXTURE_2D, gBufferUseLight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gBufferUseLight, 0);

	GLuint gAttachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
	glDrawBuffers(4, gAttachments);

	GLuint rboGDepth;
	glGenRenderbuffers(1, &rboGDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboGDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	// - Attach buffers

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboGDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
}
