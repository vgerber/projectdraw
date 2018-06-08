#include "dlight.h"

void DirectionalLight::Apply(Shader shader, std::string target)
{
	Light::Apply(shader, target);
	glUniform3f(glGetUniformLocation(shader.GetId(), (target + ".direction").c_str()), direction.x, direction.y, direction.z);
}
