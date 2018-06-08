#include "plight.h"

void PointLight::Apply(Shader shader, std::string target)
{
	Light::Apply(shader, target);
	glUniform3f(glGetUniformLocation(shader.GetProgrammId(), (target + ".position").c_str()), position.x, position.y, position.z);
	glUniform1f(glGetUniformLocation(shader.GetProgrammId(), (target + ".constant").c_str()), constant);
	glUniform1f(glGetUniformLocation(shader.GetProgrammId(), (target + ".linear").c_str()), linear);
	glUniform1f(glGetUniformLocation(shader.GetProgrammId(), (target + ".quadratic").c_str()), quadratic);
}
