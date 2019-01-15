#include "light.h"

void DepthMap::dispose()
{
	glDeleteTextures(1, &depthMap);
	glDeleteFramebuffers(1, &depthMapFBO);
}

Shader Light::getShaderShadow() {
	return shaderShadow;
}
