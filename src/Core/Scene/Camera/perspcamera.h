#pragma once

#include "camera.h"

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera();
    void setFieldOfView(float fov);
    float getFieldOfView();

    ViewFrustum getViewFrustum(int splits);
private:
    float fov = 45.0f;
protected:
    virtual void updateProjectionMatrix() override;
};