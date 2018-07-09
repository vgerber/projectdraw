#include "orthocamera.h"

glm::mat4 OrthoCamera::GetCameraMatrix() {
    return glm::ortho(-0.5f * Width, 0.5f * Width, -0.5f * Height, 0.5f * Height, NearZ, FarZ);
}
