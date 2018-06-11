#include "plight.h"

void PointLight::Apply(Shader shader, std::string target)
{
	Light::apply(shader, target);
	glUniform3f(glGetUniformLocation(shader.get_id(), (target + ".position").c_str()), position.x, position.y, position.z);
	glUniform1f(glGetUniformLocation(shader.get_id(), (target + ".constant").c_str()), constant);
	glUniform1f(glGetUniformLocation(shader.get_id(), (target + ".linear").c_str()), linear);
	glUniform1f(glGetUniformLocation(shader.get_id(), (target + ".quadratic").c_str()), quadratic);
}
