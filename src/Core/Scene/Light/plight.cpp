#include "plight.h"

PointLight::PointLight() {
	setup();
}

void PointLight::apply(Shader shader, std::string target)
{
	Light::apply(shader, target);

	glm::vec4 Mpos = mmodel * glm::vec4(size.width * 0.5f, size.height * 0.5f, size.depth * 0.5f, 1.0f);
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".position").c_str()), Mpos.x, Mpos.y, Mpos.z);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".radius").c_str()), radius);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".far_plane").c_str()), farPlane);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".constant").c_str()), attenuationConstant);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".linear").c_str()), attenuationLinear);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".quadratic").c_str()), attenuationQuadratic);
}

void PointLight::beginShadowMapping()
{
	glCullFace(GL_FRONT);
	shaderShadow.use();
	glViewport(0, 0, SHADOW_C_WIDTH, SHADOW_C_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(glGetUniformLocation(shaderShadow.getId(), "shadowMatrices[0]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
	glUniformMatrix4fv(glGetUniformLocation(shaderShadow.getId(), "shadowMatrices[1]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[1]));
	glUniformMatrix4fv(glGetUniformLocation(shaderShadow.getId(), "shadowMatrices[2]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[2]));
	glUniformMatrix4fv(glGetUniformLocation(shaderShadow.getId(), "shadowMatrices[3]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[3]));
	glUniformMatrix4fv(glGetUniformLocation(shaderShadow.getId(), "shadowMatrices[4]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[4]));
	glUniformMatrix4fv(glGetUniformLocation(shaderShadow.getId(), "shadowMatrices[5]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[5]));
	glUniform1f(glGetUniformLocation(shaderShadow.getId(), "far_plane"), farPlane);
	glUniform3f(glGetUniformLocation(shaderShadow.getId(), "lightPos"), position.x, position.y, position.z);
}

void PointLight::endShadowMapping()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);		
	glCullFace(GL_BACK);		
}

void PointLight::setPosition(glm::vec3 position)
{
	Light::setPosition(position);
	setup_shadow_cube();
}

void PointLight::setup()
{
	glGenTextures(1, &depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	for (GLuint i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_C_WIDTH, SHADOW_C_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glGenFramebuffers(1, &depthCubemapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER Framebuffer is not complete" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	aspect = (GLfloat)SHADOW_C_WIDTH / (GLfloat)SHADOW_C_HEIGHT;
	nearPlane = 1.0f;
	farPlane = 100.0f;
	shadowProj = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);

	setup_shadow_cube();

	shaderShadow = ResourceManager::loadShader(ShaderName::Depth::PerspCube);
}

void PointLight::setup_shadow_cube()
{
	shadowTransforms.clear();
	shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
}

GLuint PointLight::get_shadow_cube_map() {
	return depthCubeMap;
}