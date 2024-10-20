#pragma once

#include "../Camera/camera.h"
#include "light.h"

class DirectionalLight : public Light {
public:
  DirectionalLight();

  /// change ray direction
  void changeDirection(glm::vec3 direction);

  /// get ray direction
  glm::vec3 getDirection();

  /// set camera viewfrustum for csm
  void setViewFrustum(ViewFrustum viewFrustum);

  /// bind all opengl resource for shadowmapping
  void beginShadowMapping(int slice = 1);

  /// unbind all resources for shadowmapping and complete mapping
  void endShadowMapping();

  /// get all depth maps from cascaded shadow mapping
  std::vector<DepthMap> getDepthMaps();

  /// get maximum distance
  float getDistance();

  /// get count of csm splits/slices
  const int getCSMSlices();

  /// free opengl memory
  virtual void dispose() override;

private:
  glm::vec3 minVec;
  glm::vec3 maxVec;
  const int csmSlices = 3;
  std::vector<DepthMap> depthMaps;
  GLfloat distance = 0.0f;

  glm::vec3 direction = glm::vec3(-1.0f, -1.0f, -1.0f);
  const GLuint SHADOW_WIDTH = 1028, SHADOW_HEIGHT = 1028;

  void setup();
};