#include "light_util.h"

void setLightUniforms(std::string target, Shader shader, Light & light) {
    glUniform1i(glGetUniformLocation(shader.getId(), (target + ".drawShadow").c_str()), light.shadow);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".intensity").c_str()), light.intensity);
    glm::vec3 ambient = light.ambient;
    glm::vec3 diffuse = light.diffuse;
    glm::vec3 specular = light.specular;
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".ambient").c_str()), ambient.r, ambient.g, ambient.b);
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".diffuse").c_str()), diffuse.r, diffuse.g, diffuse.b);
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".specular").c_str()), specular.r, specular.g, specular.b);
}

void setDirectionalLightUniforms(Shader shader, DirectionalLight light, int textureOffset) {
    std::string target = "dirLight";
    setLightUniforms(target, shader, light);

    glm::vec3 direction = light.getDirection();
    glUniform3f(glGetUniformLocation(shader.getId(), (target + ".direction").c_str()), direction.x, direction.y, direction.z);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".farPlane").c_str()), light.getDistance());
	for (size_t i = 0; i < light.getDepthMaps().size(); i++) {		
		glUniformMatrix4fv(
			glGetUniformLocation(shader.getId(), (target + ".lightSpaceMatrix[" + std::to_string(i) + "]").c_str()),
			1,
			GL_FALSE,
			glm::value_ptr(light.getDepthMaps()[i].lightSpaceMatrix));

        glUniform1i(glGetUniformLocation(shader.getId(), ("dirLight.shadowMap[" + std::to_string(i) + "]").c_str()), textureOffset + i);
        glActiveTexture(GL_TEXTURE0 + textureOffset + i);
        glBindTexture(GL_TEXTURE_2D, light.getDepthMaps()[i].depthMap);
	}
}

void setPointLightUniforms(Shader shader,PointLight light, int textureOffset, int index){
    std::string target = "pointLight";
    if(index >= 0) {
        target = target + "[" + std::to_string(index) + "]";
    }
    setLightUniforms(target, shader, light);

    glm::vec3 pos = light.getPosition();
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".position").c_str()), pos.x, pos.y, pos.z);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".radius").c_str()), light.getDistance());
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".constant").c_str()), light.attenuationConstant);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".linear").c_str()), light.attenuationLinear);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".quadratic").c_str()), light.attenuationQuadratic);

    if(light.shadow) {
        std::string shadowLoc = ("pointLight[" + std::to_string(index) + "].shadowCubeMap");
        glUniform1i(glGetUniformLocation(shader.getId(), shadowLoc.c_str()), textureOffset);				
        glActiveTexture(GL_TEXTURE0 + textureOffset);
        glBindTexture(GL_TEXTURE_CUBE_MAP, light.getCubeDepthMap().depthMap);
    }
}

void setSpotLightUniforms(Shader shader, SpotLight light, int textureOffset, int index) {
    std::string target = "spotLight";
    if(index >= 0) {
        target = target + ("[" + std::to_string(index) + "]");
    }
    setLightUniforms(target, shader, light);

    glm::vec3 position = light.getPosition();
    glm::vec3 direction = light.getDirection();
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".position").c_str()), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(shader.getId(), (target + ".direction").c_str()), direction.x, direction.y, direction.z);
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".cutOff").c_str()), glm::cos(light.cutOff));
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".outerCutOff").c_str()), glm::cos(light.outerCutOff));
	glUniform1f(glGetUniformLocation(shader.getId(), (target + ".farPlane").c_str()), light.getDistance());

	if (light.shadow) {
		glUniformMatrix4fv(
			glGetUniformLocation(shader.getId(), (target + ".lightSpaceMatrix").c_str()),
			1,
			GL_FALSE,
			glm::value_ptr(light.getDepthMap().lightSpaceMatrix));
        
        std::string shadowLoc = target + ".shadowMap";
        glUniform1i(glGetUniformLocation(shader.getId(), shadowLoc.c_str()), textureOffset);
        glActiveTexture(GL_TEXTURE0 + textureOffset);
        glBindTexture(GL_TEXTURE_2D, light.getDepthMap().depthMap);
	}
}