#pragma once

#include "Resource/loader.h"
#include "Shader/shader.h"


void initCore() {
	Shader shader_geometry;
	shader_geometry.layers = {
		{ Loader::GetPath("/Shaders/geometry.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/geometry.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/geometry.geometry").c_str(), ShaderType::GEOMETRY }
	};
	shader_geometry.Load();
	Loader::Shaders[SHADER_GEOMETRY] = shader_geometry;


	Shader shader_normal_geometry;
	shader_normal_geometry.layers = {
		{ Loader::GetPath("/Shaders/normal.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/normal.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/normal.geometry").c_str(), ShaderType::GEOMETRY }
	};
	shader_normal_geometry.Load();
	Loader::Shaders["normal_geometry"] = shader_normal_geometry;


	Shader shader_depth_cube;
	shader_depth_cube.layers = {
		{ Loader::GetPath("/Shaders/cube_depth.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/cube_depth.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/cube_depth.geometry").c_str(), ShaderType::GEOMETRY }
	};
	shader_depth_cube.Load();
	Loader::Shaders[SHADER_DEPTH_CUBE] = shader_depth_cube;


	Shader shader_depth;
	shader_depth_cube.layers = {
		{ Loader::GetPath("/Shaders/depth.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/depth.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_depth.Load();
	Loader::Shaders[SHADER_DEPTH] = shader_depth;


	Shader shader_instancing;
	shader_instancing.layers = {
		{ Loader::GetPath("/Shaders/instancing.vertex").c_str(), ShaderType::VERTEX},
		{ Loader::GetPath("/Shaders/instancing.fragment").c_str(), ShaderType::FRAGMENT}
	};
	shader_instancing.Load();
	Loader::Shaders["instancing"] = shader_instancing;


	Shader shader_basic;
	shader_basic.layers = {
		{ Loader::GetPath("/Shaders/basic.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/basic.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_basic.Load();
	Loader::Shaders[SHADER_BASIC] = shader_basic;


	Shader shader1;
	shader1.layers = {
		{ Loader::GetPath("/Shaders/basic.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/lamp.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader1.Load();
	Loader::Shaders["1"] = shader1;


	Shader shader_deferred;
	shader_deferred.layers = {
		{ Loader::GetPath("/Shaders/texture.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/deferred.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_deferred.Load();
	Loader::Shaders[SHADER_DEFERRED] = shader_deferred;


	Shader shader_font;
	shader_font.layers = {
		{ Loader::GetPath("/Shaders/font.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/font.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_font.Load();
	Loader::Shaders[SHADER_FONT] = shader_font;


	Shader shader_skybox;
	shader_skybox.layers = {
		{ Loader::GetPath("/Shaders/skybox.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/skybox.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_skybox.Load();
	Loader::Shaders[SHADER_SKYBOX] = shader_skybox;

}