#include "plight.h"

PointLight::PointLight() { setup(); }

void PointLight::beginShadowMapping() {
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, SHADOW_C_WIDTH, SHADOW_C_WIDTH);
  glCullFace(GL_FRONT);
  shaderShadow.use();
  glViewport(0, 0, SHADOW_C_WIDTH, SHADOW_C_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMap.depthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);
  glUniformMatrix4fv(
      glGetUniformLocation(shaderShadow.getId(), "shadowMatrices[0]"), 1,
      GL_FALSE, glm::value_ptr(shadowTransforms[0]));
  glUniformMatrix4fv(
      glGetUniformLocation(shaderShadow.getId(), "shadowMatrices[1]"), 1,
      GL_FALSE, glm::value_ptr(shadowTransforms[1]));
  glUniformMatrix4fv(
      glGetUniformLocation(shaderShadow.getId(), "shadowMatrices[2]"), 1,
      GL_FALSE, glm::value_ptr(shadowTransforms[2]));
  glUniformMatrix4fv(
      glGetUniformLocation(shaderShadow.getId(), "shadowMatrices[3]"), 1,
      GL_FALSE, glm::value_ptr(shadowTransforms[3]));
  glUniformMatrix4fv(
      glGetUniformLocation(shaderShadow.getId(), "shadowMatrices[4]"), 1,
      GL_FALSE, glm::value_ptr(shadowTransforms[4]));
  glUniformMatrix4fv(
      glGetUniformLocation(shaderShadow.getId(), "shadowMatrices[5]"), 1,
      GL_FALSE, glm::value_ptr(shadowTransforms[5]));
  glUniform1f(glGetUniformLocation(shaderShadow.getId(), "far_plane"),
              distance);
  glm::vec3 position = getPosition();
  glUniform3f(glGetUniformLocation(shaderShadow.getId(), "lightPos"),
              position.x, position.y, position.z);
}

void PointLight::endShadowMapping() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glCullFace(GL_BACK);
}

void PointLight::translate(glm::vec3 translation) {
  Light::translate(translation);
  setupShadowCube();
}

void PointLight::setDistance(float distance) {
  this->distance = distance;
  aspect = (GLfloat)SHADOW_C_WIDTH / (GLfloat)SHADOW_C_HEIGHT;
  nearPlane = 0.0f;
  shadowProj =
      glm::perspective(glm::radians(90.0f), aspect, nearPlane, distance);
  setupShadowCube();
}

float PointLight::getDistance() { return distance; }

void PointLight::dispose() { depthMap.dispose(); }

void PointLight::setup() {
  glGenTextures(1, &depthMap.depthMap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap.depthMap);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  for (GLuint i = 0; i < 6; i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                 SHADOW_C_WIDTH, SHADOW_C_HEIGHT, 0, GL_DEPTH_COMPONENT,
                 GL_FLOAT, NULL);
  }

  glGenFramebuffers(1, &depthMap.depthMapFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMap.depthMapFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap.depthMap,
                       0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "ERROR::FRAMEBUFFER Framebuffer is not complete" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  aspect = (GLfloat)SHADOW_C_WIDTH / (GLfloat)SHADOW_C_HEIGHT;
  nearPlane = 1.0f;
  shadowProj =
      glm::perspective(glm::radians(90.0f), aspect, nearPlane, distance);

  setupShadowCube();

  shaderShadow = ResourceManager::loadShader(ShaderName::Depth::PerspCube);
}

void PointLight::setupShadowCube() {
  glm::vec3 position = getPosition();
  shadowTransforms.clear();
  shadowTransforms.push_back(
      shadowProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0),
                               glm::vec3(0.0, -1.0, 0.0)));
  shadowTransforms.push_back(
      shadowProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0),
                               glm::vec3(0.0, -1.0, 0.0)));
  shadowTransforms.push_back(
      shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0),
                               glm::vec3(0.0, 0.0, 1.0)));
  shadowTransforms.push_back(
      shadowProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0),
                               glm::vec3(0.0, 0.0, -1.0)));
  shadowTransforms.push_back(
      shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0),
                               glm::vec3(0.0, -1.0, 0.0)));
  shadowTransforms.push_back(
      shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0),
                               glm::vec3(0.0, -1.0, 0.0)));
}

DepthMap PointLight::getCubeDepthMap() { return depthMap; }