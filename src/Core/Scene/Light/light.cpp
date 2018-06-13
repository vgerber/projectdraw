#include "light.h"

void Light::apply(Shader shader, std::string target)
{
	glUniform1f(glGetUniformLocation(shader.get_id(), (target + ".drawShadow").c_str()), draw_shadow);
	glUniform1f(glGetUniformLocation(shader.get_id(), (target + ".intensity").c_str()), intensity);
	glUniform3f(glGetUniformLocation(shader.get_id(), (target + ".ambient").c_str()), ambient.r, ambient.g, ambient.b);
	glUniform3f(glGetUniformLocation(shader.get_id(), (target + ".diffuse").c_str()), diffuse.r, diffuse.g, diffuse.b);
	glUniform3f(glGetUniformLocation(shader.get_id(), (target + ".specular").c_str()), specular.r, specular.g, specular.b);
}
