#include "light.h"

void Light::apply(Shader shader, std::string target)
{
	glUniform1i(glGetUniformLocation(shader.get_id(), (target + ".is_on").c_str()), is_on);
	glUniform3f(glGetUniformLocation(shader.get_id(), (target + ".ambient").c_str()), ambient.r, ambient.g, ambient.b);
	glUniform3f(glGetUniformLocation(shader.get_id(), (target + ".diffuse").c_str()), diffuse.r, diffuse.g, diffuse.b);
	glUniform3f(glGetUniformLocation(shader.get_id(), (target + ".specular").c_str()), specular.r, specular.g, specular.b);
}

void Light::on()
{
	is_on = true;
}

void Light::off()
{
	is_on = false;
}
