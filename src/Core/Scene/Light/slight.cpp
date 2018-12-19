#include "slight.h"

SpotLight::SpotLight()
{
	setup();
}

void SpotLight::beginShadowMapping()
{
	glm::vec3 position = getPosition();
	glm::mat4 lightView = glm::lookAt(position, position + glm::normalize(direction), glm::vec3(0.0, 1.0, 0.0));
	depthMap.lightSpaceMatrix =  glm::perspective(glm::radians(glm::degrees(outerCutOff) * 2.f), 1.0f, 0.1f, distance) * lightView;

	shaderShadow.use();

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMap.depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUniform1f(glGetUniformLocation(shaderShadow.getId(), "farPlane"), distance);
	glUniform3f(glGetUniformLocation(shaderShadow.getId(), "lightPos"), position.x, position.y, position.z);

	glUniformMatrix4fv(
		glGetUniformLocation(shaderShadow.getId(), "lightSpaceMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(depthMap.lightSpaceMatrix));
}

void SpotLight::endShadowMapping()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint SpotLight::getShadowMap()
{
	return depthMap.depthMap;
}

void SpotLight::setDirection(glm::vec3 direction)
{
	this->direction = direction;
}

glm::vec3 SpotLight::getDirection() {
	return direction;
}

void SpotLight::setDistance(float distance)
{
	this->distance = distance;
}

float SpotLight::getDistance() {
	return distance;
}

void SpotLight::apply(Shader shader, std::string target)
{
	Light::apply(shader, target);
	glm::vec3 position = getPosition();
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".position").c_str()), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".direction").c_str()), direction.x, direction.y, direction.z);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".cutOff").c_str()), glm::cos(cutOff));
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".outerCutOff").c_str()), glm::cos(outerCutOff));
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".farPlane").c_str()), distance);

	if (shadow) {
		glUniformMatrix4fv(
			glGetUniformLocation(shader.getId(), (target + ".lightSpaceMatrix").c_str()),
			1,
			GL_FALSE,
			glm::value_ptr(depthMap.lightSpaceMatrix));
	}
}

void SpotLight::setCutOff(float inner, float outer)
{
	cutOff = glm::radians(inner);
	outerCutOff = glm::radians(outer);
}

void SpotLight::dispose()
{
	depthMap.dispose();
}

void SpotLight::setup()
{
	glGenFramebuffers(1, &depthMap.depthMapFBO);
	glGenTextures(1, &depthMap.depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap.depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat bordercolor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMap.depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap.depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	shaderShadow = ResourceManager::loadShader(ShaderName::Depth::Persp);
}
