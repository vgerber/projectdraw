#pragma once
// #define GLEW_STATIC
#include <GL/glew.h>

// base libs
#include <btBulletDynamicsCommon.h>

// Resources
#include "Resource/loader.h"
#include "Resource/resourcemanager.h"
#include "Shader/shader.h"
#include "core.h"

// basic imports
#include "Scene/Scenes/scene.h"
#include "Scene/Scenes/scene2d.h"

// camera
#include "Scene/Camera/camera.h"
#include "Scene/Camera/orthocamera.h"
#include "Scene/Camera/perspcamera.h"

// primitves
#include "Mesh/Primitives/primitives.h"

// particles
#include "Core/Scene/Particle/particles.h"

// lighting
#include "Scene/Light/dlight.h"
#include "Scene/Light/plight.h"
#include "Scene/Light/slight.h"

// font
#include "Scene/Text/text.h"

// util
#include "Util/Debug/vector.h"
#include "Util/Mouse/mouse.h"
