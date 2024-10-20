#pragma once
// #define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Resource/resourcemanager.h"
#include "Core/Shader/shader.h"
#include "Core/Util/Config/configurator.h"
#include "Core/Util/Debug/logging.h"

void initCore();

void loadDeferredShaders();

void loadForwardShaders();

void loadDepthShaders();

void loadTextureShaders();

void loadPostprocessingShaders();

void loadFlatShaders();

void loadExperimentalShaders();

void clearScreen(glm::vec4 color);
