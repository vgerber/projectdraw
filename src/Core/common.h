#pragma once

//base libs
#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>

#include <btBulletDynamicsCommon.h>
#include <btBulletDynamicsCommon.h>


//Resources
#include "core.h"
#include "Resource/loader.h"
#include "Shader/shader.h"

//basic imports
#include "Window/window.h"

#include "Scene/scene.h"

//camera
#include "Scene/Camera/camera.h"
#include "Scene/Camera/perspcamera.h"
#include "Scene/Camera/orthocamera.h"

//primitves
#include "Scene/Primitive/primitives.h"

//lighting
#include "Scene/Light/dlight.h"
#include "Scene/Light/plight.h"

//font
#include "Scene/Text/text.h"

//util
#include "Util/Debug/vector.h"
#include "Util/Mouse/mouse.h"
