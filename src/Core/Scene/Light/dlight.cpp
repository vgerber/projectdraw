#include "dlight.h"

DirectionalLight::DirectionalLight()
{
	setup();
}

void DirectionalLight::change_direction(glm::vec3 direction)
{
	//this->direction = direction;
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
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".farPlane").c_str()), farPlane);
	for (int i = 0; i < depthMaps.size(); i++) {		
		glUniformMatrix4fv(
			glGetUniformLocation(shader.getId(), (target + ".lightSpaceMatrix[" + std::to_string(i) + "]").c_str()),
			1,
			GL_FALSE,
			glm::value_ptr(depthMaps[i].lightSpaceMatrix));
	}
}

void DirectionalLight::setViewFrustum(ViewFrustum viewFrustum)
{
	assert(viewFrustum.splits.size() == csmSlices + 1);

	farPlane = viewFrustum.farZ;

	float maxX, minX, maxY, minY, maxZ, minZ;
	bool isInit = true;
	for (int i = 0; i < csmSlices; i++) {
		for (auto corner : viewFrustum.splits[i+1])
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

		for (auto corner : viewFrustum.splits[i])
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
		for (auto corner : viewFrustum.splits[i+1])
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

		for (auto corner : viewFrustum.splits[i])
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
		depthMaps[i].lightSpaceMatrix = glm::ortho(minX, maxX, minY, maxY, -maxZ, -minZ) * lightView;
	}
}

void DirectionalLight::begin_shadow_mapping(int slice)
{
	Shaders[SHADER_DEPTH].use();

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMaps[slice].depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(
		glGetUniformLocation(Shaders[SHADER_DEPTH].getId(), "lightSpaceMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(depthMaps[slice].lightSpaceMatrix));
}

void DirectionalLight::end_shadow_mapping()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint DirectionalLight::getShadowMap(int slice)
{
	return depthMaps[slice].depthMap;
}

const int DirectionalLight::getCSMSlices()
{
	return csmSlices;
}

void DirectionalLight::dispose()
{
	for (auto dm : depthMaps) {
		glDeleteFramebuffers(1, &dm.depthMapFBO);
		glDeleteTextures(1, &dm.depthMap);
	}
	depthMaps.clear();
}

void DirectionalLight::setup()
{
	

	for (int i = 0; i < csmSlices; i++) {
		DepthMap dm;
		glGenFramebuffers(1, &dm.depthMapFBO);
		glGenTextures(1, &dm.depthMap);
		glBindTexture(GL_TEXTURE_2D, dm.depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		GLfloat bordercolor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);

		glBindFramebuffer(GL_FRAMEBUFFER, dm.depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dm.depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		depthMaps.push_back(dm);
	}
}