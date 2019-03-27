#pragma once

#include "camera.h"

class PerspectiveCamera : public Camera {
public:
    /**
     * @brief Construct a new Perspective Camera object
     * 
     */
    PerspectiveCamera();

    /**
     * @brief Set the Field Of View 
     * 
     * @param fov 
     */
    void setFieldOfView(float fov);

    /**
     * @brief Get the Field Of View 
     * 
     * @return float 
     */
    float getFieldOfView();

    /**
     * @brief Get the ViewFrustum
     * 
     * @param splits splits frustum in subregions
     * @return ViewFrustum 
     */
    ViewFrustum getViewFrustum(int splits);
private:
    float fov = 45.0f;
protected:
    /**
     * @brief 
     * 
     */
    virtual void updateProjectionMatrix() override;
};