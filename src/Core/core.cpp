#include "core.h"

void init_core() {
	Shader shader_geometry;
	shader_geometry.layers = {
		{ Loader::GetPath("/Shaders/geometry.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/geometry.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/geometry.geometry").c_str(), ShaderType::GEOMETRY }
	};
	shader_geometry.load();
	Shaders[SHADER_GEOMETRY] = shader_geometry;


	Shader shader_normal_geometry;
	shader_normal_geometry.layers = {
		{ Loader::GetPath("/Shaders/normal.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/normal.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/normal.geometry").c_str(), ShaderType::GEOMETRY }
	};
	shader_normal_geometry.load();
	Shaders[SHADER_DEFFERED_NORMALS] = shader_normal_geometry;


	Shader shader_depth_cube;
	shader_depth_cube.layers = {
		{ Loader::GetPath("/Shaders/cube_depth.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/cube_depth.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/cube_depth.geometry").c_str(), ShaderType::GEOMETRY }
	};
	shader_depth_cube.load();
	Shaders[SHADER_DEPTH_CUBE] = shader_depth_cube;


	Shader shader_depth;
	shader_depth.layers = {
		{ Loader::GetPath("/Shaders/depth.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/depth.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_depth.load();
	Shaders[SHADER_DEPTH] = shader_depth;


	Shader shader_instancing;
	shader_instancing.layers = {
		{ Loader::GetPath("/Shaders/instancing.vertex").c_str(), ShaderType::VERTEX},
		{ Loader::GetPath("/Shaders/instancing.fragment").c_str(), ShaderType::FRAGMENT}
	};
	shader_instancing.load();
	Shaders["instancing"] = shader_instancing;


	Shader shader_basic;
	shader_basic.layers = {
		{ Loader::GetPath("/Shaders/basic.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/basic.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_basic.load();
	Shaders[SHADER_BASIC] = shader_basic;


	Shader shader_light;
	shader_light.layers = {
		{ Loader::GetPath("/Shaders/light.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/light.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_light.load();
	Shaders[SHADER_DEFFERED_LIGHT] = shader_light;


	Shader shader_deferred;
	shader_deferred.layers = {
		{ Loader::GetPath("/Shaders/texture.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/deferred.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_deferred.load();
	Shaders[SHADER_DEFERRED] = shader_deferred;


	Shader shader_font;
	shader_font.layers = {
		{ Loader::GetPath("/Shaders/font.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/font.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_font.load();
	Shaders[SHADER_FONT] = shader_font;


	Shader shader_skybox;
	shader_skybox.layers = {
		{ Loader::GetPath("/Shaders/skybox.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/skybox.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_skybox.load();
	Shaders[SHADER_SKYBOX] = shader_skybox;


	glUniformBlockBinding(Shaders[SHADER_BASIC].get_id(), glGetUniformBlockIndex(Shaders[SHADER_BASIC].get_id(), "Matrices"), 0);
	glUniformBlockBinding(Shaders[SHADER_FONT].get_id(), glGetUniformBlockIndex(Shaders[SHADER_FONT].get_id(), "Matrices"), 0);
	glUniformBlockBinding(Shaders[SHADER_SKYBOX].get_id(), glGetUniformBlockIndex(Shaders[SHADER_SKYBOX].get_id(), "Matrices"), 0);
	glUniformBlockBinding(Shaders[SHADER_DEFFERED_LIGHT].get_id(), glGetUniformBlockIndex(Shaders[SHADER_DEFFERED_LIGHT].get_id(), "Matrices"), 0);
}