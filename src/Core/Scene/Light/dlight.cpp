#include "dlight.h"

DirectionalLight::DirectionalLight() { setup(); }

void DirectionalLight::changeDirection(glm::vec3 direction) {
  this->direction = direction;
}

glm::vec3 DirectionalLight::getDirection() { return direction; }

void DirectionalLight::setViewFrustum(ViewFrustum viewFrustum) {
  assert(viewFrustum.splits.size() == csmSlices + 1);

  distance = viewFrustum.farZ;

  float maxX, minX, maxY, minY, maxZ, minZ;
  bool isInit = true;
  for (int i = 0; i < csmSlices; i++) {
    for (auto corner : viewFrustum.splits[i + 1]) {
      if (isInit) {
        maxX = corner.x;
        minX = corner.x;
        maxY = corner.y;
        minY = corner.y;
        maxZ = corner.z;
        minZ = corner.z;
        isInit = false;
        continue;
      }
      if (corner.x < minX) {
        minX = corner.x;
      }
      if (corner.x > maxX) {
        maxX = corner.x;
      }
      if (corner.y < minY) {
        minY = corner.y;
      }
      if (corner.y > maxY) {
        maxY = corner.y;
      }
      if (corner.z < minZ) {
        minZ = corner.z;
      }
      if (corner.z > maxZ) {
        maxZ = corner.z;
      }
    }

    for (auto corner : viewFrustum.splits[i]) {
      if (isInit) {
        maxX = corner.x;
        minX = corner.x;
        maxY = corner.y;
        minY = corner.y;
        maxZ = corner.z;
        minZ = corner.z;
        isInit = false;
        continue;
      }
      if (corner.x < minX) {
        minX = corner.x;
      }
      if (corner.x > maxX) {
        maxX = corner.x;
      }
      if (corner.y < minY) {
        minY = corner.y;
      }
      if (corner.y > maxY) {
        maxY = corner.y;
      }
      if (corner.z < minZ) {
        minZ = corner.z;
      }
      if (corner.z > maxZ) {
        maxZ = corner.z;
      }
    }

    maxVec = glm::vec4(maxX, maxY, maxZ, 1.0f);
    minVec = glm::vec4(minX, minY, minZ, 1.0f);

    glm::vec3 center = (maxVec + minVec) * glm::vec3(0.5f);

    glm::mat4 lightView =
        glm::lookAt(center, center + direction, glm::vec3(0.0f, 1.0f, 0.0f));

    isInit = true;
    for (auto corner : viewFrustum.splits[i + 1]) {
      corner = lightView * glm::vec4(corner, 1.0f);
      if (isInit) {
        maxX = corner.x;
        minX = corner.x;
        maxY = corner.y;
        minY = corner.y;
        maxZ = corner.z;
        minZ = corner.z;
        isInit = false;
        continue;
      }
      if (corner.x < minX) {
        minX = corner.x;
      }
      if (corner.x > maxX) {
        maxX = corner.x;
      }
      if (corner.y < minY) {
        minY = corner.y;
      }
      if (corner.y > maxY) {
        maxY = corner.y;
      }
      if (corner.z < minZ) {
        minZ = corner.z;
      }
      if (corner.z > maxZ) {
        maxZ = corner.z;
      }
    }

    for (auto corner : viewFrustum.splits[i]) {
      corner = lightView * glm::vec4(corner, 1.0f);
      if (isInit) {
        maxX = corner.x;
        minX = corner.x;
        maxY = corner.y;
        minY = corner.y;
        maxZ = corner.z;
        minZ = corner.z;
        isInit = false;
        continue;
      }
      if (corner.x < minX) {
        minX = corner.x;
      }
      if (corner.x > maxX) {
        maxX = corner.x;
      }
      if (corner.y < minY) {
        minY = corner.y;
      }
      if (corner.y > maxY) {
        maxY = corner.y;
      }
      if (corner.z < minZ) {
        minZ = corner.z;
      }
      if (corner.z > maxZ) {
        maxZ = corner.z;
      }
    }
    depthMaps[i].lightSpaceMatrix =
        glm::ortho(minX, maxX, minY, maxY, -maxZ, -minZ) * lightView;
  }
}

void DirectionalLight::beginShadowMapping(int slice) {
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_FRONT);
  shaderShadow.use();

  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMaps[slice].depthMapFBO);

  glClear(GL_DEPTH_BUFFER_BIT);

  glUniformMatrix4fv(
      glGetUniformLocation(shaderShadow.getId(), "lightSpaceMatrix"), 1,
      GL_FALSE, glm::value_ptr(depthMaps[slice].lightSpaceMatrix));
}

void DirectionalLight::endShadowMapping() {
  glCullFace(GL_BACK);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glEnable(GL_DEPTH_TEST);
}

std::vector<DepthMap> DirectionalLight::getDepthMaps() { return depthMaps; }

float DirectionalLight::getDistance() { return distance; }

const int DirectionalLight::getCSMSlices() { return csmSlices; }

void DirectionalLight::dispose() {
  for (auto dm : depthMaps) {
    dm.dispose();
  }
  depthMaps.clear();
}

void DirectionalLight::setup() {
  shaderShadow = ResourceManager::loadShader(ShaderName::Depth::Ortho);

  for (int i = 0; i < csmSlices; i++) {
    DepthMap dm;
    glGenFramebuffers(1, &dm.depthMapFBO);
    glGenTextures(1, &dm.depthMap);
    glBindTexture(GL_TEXTURE_2D, dm.depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SHADOW_WIDTH,
                 SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat bordercolor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);

    glBindFramebuffer(GL_FRAMEBUFFER, dm.depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           dm.depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    depthMaps.push_back(dm);
  }
}