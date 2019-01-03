#pragma once

#include "camera.h"


class OrthographicCamera : public Camera {
public:
	OrthographicCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f));

    ViewFrustum getViewFrustum(int splits);

protected:
    virtual void updateProjectionMatrix() override;
};