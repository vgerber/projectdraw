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
		{ Loader::GetPath("/Shaders/Depth/cube_depth.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Depth/cube_depth.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/Depth/cube_depth.geometry").c_str(), ShaderType::GEOMETRY }
	};
	shader_depth_cube.load();
	Shaders[SHADER_DEPTH_CUBE] = shader_depth_cube;


	Shader shader_depth;
	shader_depth.layers = {
		{ Loader::GetPath("/Shaders/Depth/depth.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Depth/depth.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_depth.load();
	Shaders[SHADER_DEPTH] = shader_depth;


	/*Shader shader_instancing;
	shader_instancing.layers = {
		{ Loader::GetPath("/Shaders/instancing.vertex").c_str(), ShaderType::VERTEX},
		{ Loader::GetPath("/Shaders/instancing.fragment").c_str(), ShaderType::FRAGMENT}
	};
	shader_instancing.load();
	Shaders["instancing"] = shader_instancing;
	*/


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
		{ Loader::GetPath("/Shaders/Deferred/deferred.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/Light/dlight_shadow.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shader_deferred.load();
	Shaders[SHADER_DEFERRED] = shader_deferred;

	Shader shader_deferred_plight_nos;
	shader_deferred_plight_nos.layers = {
		{ Loader::GetPath("/Shaders/texture.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Deferred/deferred_plight_nos.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/Light/plight.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shader_deferred_plight_nos.load();
	Shaders[SHADER_DEFFERED_PLIGHT_NOS] = shader_deferred_plight_nos;

	Shader shader_deferred_slight_nos;
	shader_deferred_slight_nos.layers = {
		{ Loader::GetPath("/Shaders/texture.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Deferred/deferred_slight_nos.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/Light/slight.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shader_deferred_slight_nos.load();
	Shaders[SHADER_DEFFERED_SLIGHT_NOS] = shader_deferred_slight_nos;


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

	Shader shader_deferred_geometry;
	shader_deferred_geometry.layers = {
		{ Loader::GetPath("/Shaders/Geometry/geometry.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Geometry/geometry.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_deferred_geometry.load();
	Shaders[SHADER_DEFFERED_GEOMETRY] = shader_deferred_geometry;

	Shader shader_texture;
	shader_texture.layers = {
		{ Loader::GetPath("/Shaders/texture.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/texture.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_texture.load();
	Shaders[SHADER_TEXTURE] = shader_texture;



	Shader shader_perspective_depth;
	shader_perspective_depth.layers = {
		{ Loader::GetPath("/Shaders/Depth/depth_persp.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Depth/depth_persp.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_perspective_depth.load();
	Shaders[SHADER_DEPTH_PERSP] = shader_perspective_depth;

	Shader shader_filter_blur;
	shader_filter_blur.layers = {
		{ Loader::GetPath("/Shaders/texture.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Filter/blur.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_filter_blur.load();
	Shaders[SHADER_FILTER_BLUR] = shader_filter_blur;


	glUniformBlockBinding(Shaders[SHADER_BASIC].getId(), glGetUniformBlockIndex(Shaders[SHADER_BASIC].getId(), "Matrices"), 0);
	glUniformBlockBinding(Shaders[SHADER_FONT].getId(), glGetUniformBlockIndex(Shaders[SHADER_FONT].getId(), "Matrices"), 0);
	glUniformBlockBinding(Shaders[SHADER_SKYBOX].getId(), glGetUniformBlockIndex(Shaders[SHADER_SKYBOX].getId(), "Matrices"), 0);
	glUniformBlockBinding(Shaders[SHADER_DEFFERED_LIGHT].getId(), glGetUniformBlockIndex(Shaders[SHADER_DEFFERED_LIGHT].getId(), "Matrices"), 0);
	glUniformBlockBinding(Shaders[SHADER_DEFFERED_GEOMETRY].getId(), glGetUniformBlockIndex(Shaders[SHADER_DEFFERED_GEOMETRY].getId(), "Matrices"), 0);
}

glm::vec3 toVec3(btVector3 vec)
{
	return glm::vec3(vec.getX(), vec.getY(), vec.getZ());
}

btVector3 toBtVec3(glm::vec3 vec)
{
	return btVector3(vec.x, vec.y, vec.z);
}
