#include "light.h"

Light::Light()
{
}

void Light::apply(Shader shader, std::string target)
{
	glUniform1i(glGetUniformLocation(shader.getId(), (target + ".drawShadow").c_str()), shadow);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".intensity").c_str()), intensity);
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".ambient").c_str()), ambient.r, ambient.g, ambient.b);
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".diffuse").c_str()), diffuse.r, diffuse.g, diffuse.b);
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".specular").c_str()), specular.r, specular.g, specular.b);
}

Shader Light::getShaderShadow() {
	return shaderShadow;
}

void DepthMap::dispose()
{
	glDeleteTextures(1, &depthMap);
	glDeleteFramebuffers(1, &depthMapFBO);
}
