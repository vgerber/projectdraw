#include "dlight.h"

DirectionalLight::DirectionalLight()
{
	setup();
}

void DirectionalLight::change_direction(glm::vec3 direction) {
	this->direction = direction;
	lightView = glm::lookAt(-direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
}

glm::vec3 DirectionalLight::get_direction() {
	return direction;
}

void DirectionalLight::apply(Shader shader, std::string target)
{
	Light::apply(shader, target);
	glUniform3f(glGetUniformLocation(shader.get_id(), (target + ".direction").c_str()), direction.x, direction.y, direction.z);
	glUniformMatrix4fv(
		glGetUniformLocation(shader.get_id(), (target + ".lightSpaceMatrix").c_str()),
		1,
		GL_FALSE,
		glm::value_ptr(lightSpaceMatrix));
}

void DirectionalLight::begin_shadow_mapping()
{
	glCullFace(GL_FRONT);
	Shaders[SHADER_DEPTH].use();	

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(
		glGetUniformLocation(Shaders[SHADER_DEPTH].get_id(), "lightSpaceMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(lightSpaceMatrix));
}

void DirectionalLight::end_shadow_mapping()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

GLuint DirectionalLight::get_shadow_map()
{
	return depthMap;
}

void DirectionalLight::setup() {
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat bordercolor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	near_plane = 0.1f;
	far_plane = 100.0f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(-direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
}