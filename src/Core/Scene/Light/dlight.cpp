#include "dlight.h"

DirectionalLight::DirectionalLight()
{
	setup();
}

void DirectionalLight::change_direction(glm::vec3 direction)
{
	this->direction = direction;
	//lightView = glm::lookAt(-direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//lightSpaceMatrix = lightProjection * lightView;
}

glm::vec3 DirectionalLight::get_direction()
{
	return direction;
}

void DirectionalLight::apply(Shader shader, std::string target)
{
	Light::apply(shader, target);
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".direction").c_str()), direction.x, direction.y, direction.z);
	glUniformMatrix4fv(
		glGetUniformLocation(shader.getId(), (target + ".lightSpaceMatrix").c_str()),
		1,
		GL_FALSE,
		glm::value_ptr(lightSpaceMatrix));
}

void DirectionalLight::setViewFrustum(ViewFrustum viewFrustum)
{

	float maxX, minX, maxY, minY, maxZ, minZ;
	bool isInit = true;
	for (auto corner : viewFrustum.farCorners)
	{
		if (isInit)
		{
			maxX = corner.x;
			minX = corner.x;
			maxY = corner.y;
			minY = corner.y;
			maxZ = corner.z;
			minZ = corner.z;
			isInit = false;
			continue;
		}
		if (corner.x < minX)
		{
			minX = corner.x;
		}
		if (corner.x > maxX)
		{
			maxX = corner.x;
		}
		if (corner.y < minY)
		{
			minY = corner.y;
		}
		if (corner.y > maxY)
		{
			maxY = corner.y;
		}
		if (corner.z < minZ)
		{
			minZ = corner.z;
		}
		if (corner.z > maxZ)
		{
			maxZ = corner.z;
		}
	}

	for (auto corner : viewFrustum.nearCorners)
	{
		if (isInit)
		{
			maxX = corner.x;
			minX = corner.x;
			maxY = corner.y;
			minY = corner.y;
			maxZ = corner.z;
			minZ = corner.z;
			isInit = false;
			continue;
		}
		if (corner.x < minX)
		{
			minX = corner.x;
		}
		if (corner.x > maxX)
		{
			maxX = corner.x;
		}
		if (corner.y < minY)
		{
			minY = corner.y;
		}
		if (corner.y > maxY)
		{
			maxY = corner.y;
		}
		if (corner.z < minZ)
		{
			minZ = corner.z;
		}
		if (corner.z > maxZ)
		{
			maxZ = corner.z;
		}
	}

	maxVec = glm::vec4(maxX, maxY, maxZ, 1.0f);
	minVec = glm::vec4(minX, minY, minZ, 1.0f);

	glm::vec3 center = (maxVec + minVec) * glm::vec3(0.5f);


	glm::mat4 lightView = glm::lookAt(center, center + direction, glm::vec3(0.0f, 1.0f, 0.0f));

	isInit = true;
	for (auto corner : viewFrustum.farCorners)
	{
		corner = lightView * glm::vec4(corner, 1.0f);
		if (isInit)
		{
			maxX = corner.x;
			minX = corner.x;
			maxY = corner.y;
			minY = corner.y;
			maxZ = corner.z;
			minZ = corner.z;
			isInit = false;
			continue;
		}
		if (corner.x < minX)
		{
			minX = corner.x;
		}
		if (corner.x > maxX)
		{
			maxX = corner.x;
		}
		if (corner.y < minY)
		{
			minY = corner.y;
		}
		if (corner.y > maxY)
		{
			maxY = corner.y;
		}
		if (corner.z < minZ)
		{
			minZ = corner.z;
		}
		if (corner.z > maxZ)
		{
			maxZ = corner.z;
		}
	}

	for (auto corner : viewFrustum.nearCorners)
	{
		corner = lightView * glm::vec4(corner, 1.0f);
		if (isInit)
		{
			maxX = corner.x;
			minX = corner.x;
			maxY = corner.y;
			minY = corner.y;
			maxZ = corner.z;
			minZ = corner.z;
			isInit = false;
			continue;
		}
		if (corner.x < minX)
		{
			minX = corner.x;
		}
		if (corner.x > maxX)
		{
			maxX = corner.x;
		}
		if (corner.y < minY)
		{
			minY = corner.y;
		}
		if (corner.y > maxY)
		{
			maxY = corner.y;
		}
		if (corner.z < minZ)
		{
			minZ = corner.z;
		}
		if (corner.z > maxZ)
		{
			maxZ = corner.z;
		}
	}

	lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
	//lightView = glm::lookAt(-direction, glm::vec3(0.0), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
}

void DirectionalLight::begin_shadow_mapping()
{
	glCullFace(GL_FRONT);
	Shaders[SHADER_DEPTH].use();

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(
		glGetUniformLocation(Shaders[SHADER_DEPTH].getId(), "lightSpaceMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(lightSpaceMatrix));
}

void DirectionalLight::end_shadow_mapping()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

GLuint DirectionalLight::get_shadow_map()
{
	return depthMap;
}

void DirectionalLight::setup()
{
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat bordercolor[] = {1.0, 1.0, 1.0, 1.0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	near_plane = 0.1f;
	far_plane = 100.0f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f);
	lightView = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
}