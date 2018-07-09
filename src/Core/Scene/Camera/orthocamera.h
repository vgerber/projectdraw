#pragma once

#include "camera.h"


class OrthoCamera : public Camera {
    glm::mat4 getCameraMatrix();

    ViewFrustum getViewFrustum(int splits);
};