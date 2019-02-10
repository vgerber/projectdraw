#pragma once
//#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Resource/resourcemanager.h"
#include "Shader/shader.h"

void initCore();

void loadDeferredShaders();

void loadForwardShaders();

void loadDepthShaders();

void loadTextureShaders();

void loadPostprocessingShaders();

void loadExperimentalShaders();

void clearScreen(glm::vec4 color);
