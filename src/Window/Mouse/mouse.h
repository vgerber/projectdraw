#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>

#include "Core/Scene/Camera/camera.h"
#include "Core/Scene/Primitive/geometry.h"
#include "Window/window.h"

Point getRelativeMousePosition(Window &window);

std::pair<Point, Point> getCameraMousePosition(Window &window, Camera &camera);
