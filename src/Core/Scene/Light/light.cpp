#include "light.h"

void Light::Apply(Shader shader, std::string target)
{
	glUniform3f(glGetUniformLocation(shader.GetProgrammId(), (target + ".ambient").c_str()), ambient.r, ambient.g, ambient.b);
	glUniform3f(glGetUniformLocation(shader.GetProgrammId(), (target + ".diffuse").c_str()), diffuse.r, diffuse.g, diffuse.b);
	glUniform3f(glGetUniformLocation(shader.GetProgrammId(), (target + ".specular").c_str()), specular.r, specular.g, specular.b);
}
