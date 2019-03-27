#pragma once

#include "camera.h"


class OrthographicCamera : public Camera {
public:
    /**
     * @brief Construct a new Orthographic Camera object
     * 
     * @param position 
     * @param forward 
     * @param up 
     */
	OrthographicCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f));

    /**
     * @brief Get the ViewFrustum
     * 
     * @param splits number of subregions in frustum
     * @return ViewFrustum 
     */
    ViewFrustum getViewFrustum(int splits);

protected:
    /**
     * @brief recalculate projection matrix
     * 
     */
    virtual void updateProjectionMatrix() override;
};