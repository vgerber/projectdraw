#include "light.h"

Light::Light()
{
	shader = Shaders[SHADER_DEFFERED_LIGHT];
}

void Light::updateModel()
{
	Drawable::updateModel();
	//glm::vec3 center_translate = glm::vec3(0.5f * size.width, 0.5f * size.height, 0.5f * size.depth);
	//mmodel = glm::translate(mmodel, -center_translate);
}

void Light::apply(Shader shader, std::string target)
{
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".drawShadow").c_str()), draw_shadow);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".intensity").c_str()), intensity);
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".ambient").c_str()), ambient.r, ambient.g, ambient.b);
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".diffuse").c_str()), diffuse.r, diffuse.g, diffuse.b);
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".specular").c_str()), specular.r, specular.g, specular.b);
}

void DepthMap::dispose()
{
	glDeleteTextures(1, &depthMap);
	glDeleteFramebuffers(1, &depthMapFBO);
}
