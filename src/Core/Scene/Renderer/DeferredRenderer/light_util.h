#pragma once
#include "Core/Scene/Light/dlight.h"
#include "Core/Scene/Light/plight.h"
#include "Core/Scene/Light/slight.h"

///Used in functions below for setting uniform paramaters from the base class (light)
///@param target name of uniform variable/array
void setLightUniforms(std::string target, Shader shader, Light & light);

///Set uniform parameters for directional lights
void setDirectionalLightUniforms(Shader shader, DirectionalLight light, int textureOffset);

///Set uniform parameters for point lights
///if index is -1 we will assume that the shader uses a single non array pointlight
void setPointLightUniforms(Shader shader, PointLight light, int textureOffset, int index = -1);

///Set uniform parameters for spot lights
///if index is -1 we will assume that the shader uses a single non array spotlight
void setSpotLightUniforms(Shader shader, SpotLight light, int textureOffset, int index = -1);