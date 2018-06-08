#include "plight.h"

void PointLight::Apply(Shader shader, std::string target)
{
	Light::Apply(shader, target);
	glUniform3f(glGetUniformLocation(shader.GetId(), (target + ".position").c_str()), position.x, position.y, position.z);
	glUniform1f(glGetUniformLocation(shader.GetId(), (target + ".constant").c_str()), constant);
	glUniform1f(glGetUniformLocation(shader.GetId(), (target + ".linear").c_str()), linear);
	glUniform1f(glGetUniformLocation(shader.GetId(), (target + ".quadratic").c_str()), quadratic);
}
