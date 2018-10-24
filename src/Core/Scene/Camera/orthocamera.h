#pragma once

#include "camera.h"


class OrthographicCamera : public Camera {
public:
	OrthographicCamera();
	OrthographicCamera(glm::vec3 position, glm::vec3 up, glm::vec3 front);

    glm::mat4 getCameraMatrix();
    ViewFrustum getViewFrustum(int splits);
};