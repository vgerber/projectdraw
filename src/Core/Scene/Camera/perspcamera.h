#pragma once

#include "camera.h"

class PerspectiveCamera : public Camera {
public:
    GLfloat FOV = 45.0f;

    glm::mat4 getCameraMatrix();
    ViewFrustum getViewFrustum(int splits);
private:
};