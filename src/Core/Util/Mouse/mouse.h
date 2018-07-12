#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "../../Scene/Primitive/geometry.h"
#include "../../Window/window.h"
#include "../../Scene/Camera/camera.h"

Point getRelativeMousePosition(Window &window);

std::pair<Point, Point> getCameraMousePosition(Window &window, Camera &camera);
