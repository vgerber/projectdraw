#include "core.h"

void initCore() {

	//depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);



	glEnable(GL_CULL_FACE);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_MULTISAMPLE);

	glEnable(GL_FRAMEBUFFER_SRGB);


	glEnable(GL_LINE_SMOOTH);

	//glEnable(GL_LINE_STIPPLE);	
	//glLineStipple(1, 0xAAAA);



	Shader shader_geometry;
	shader_geometry.layers = {
		{ Loader::GetPath("/Shaders/geometry.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/geometry.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/geometry.geometry").c_str(), ShaderType::GEOMETRY }
	};
	shader_geometry.load();
	Shaders[SHADER_GEOMETRY] = shader_geometry;
	ResourceManager::storeShader(SHADER_GEOMETRY, shader_geometry);


	Shader shader_normal_geometry;
	shader_normal_geometry.layers = {
		{ Loader::GetPath("/Shaders/normal.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/normal.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/normal.geometry").c_str(), ShaderType::GEOMETRY }
	};
	shader_normal_geometry.load();
	Shaders[SHADER_DEFFERED_NORMALS] = shader_normal_geometry;
	ResourceManager::storeShader(SHADER_DEFFERED_NORMALS, shader_normal_geometry);
	
	Shader shader_depth_cube;
	shader_depth_cube.layers = {
		{ Loader::GetPath("/Shaders/Depth/cube_depth.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Depth/cube_depth.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/Depth/cube_depth.geometry").c_str(), ShaderType::GEOMETRY }
	};
	shader_depth_cube.load();
	Shaders[SHADER_DEPTH_CUBE] = shader_depth_cube;
	ResourceManager::storeShader(SHADER_DEPTH_CUBE, shader_depth_cube);

	Shader shader_depth;
	shader_depth.layers = {
		{ Loader::GetPath("/Shaders/Depth/depth.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Depth/depth.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_depth.load();
	Shaders[SHADER_DEPTH] = shader_depth;
	ResourceManager::storeShader(SHADER_DEPTH, shader_depth);

	Shader shader_basic;
	shader_basic.layers = {
		{ Loader::GetPath("/Shaders/basic.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/basic.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_basic.load();
	Shaders[SHADER_BASIC] = shader_basic;
	ResourceManager::storeShader(SHADER_BASIC, shader_basic);
	
	Shader shader_light;
	shader_light.layers = {
		{ Loader::GetPath("/Shaders/light.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/light.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_light.load();
	Shaders[SHADER_DEFFERED_LIGHT] = shader_light;
	ResourceManager::storeShader(SHADER_DEFFERED_LIGHT, shader_light);

	Shader shader_deferred;
	shader_deferred.layers = {
		{ Loader::GetPath("/Shaders/texture.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Deferred/deferred.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/Light/dlight_shadow.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shader_deferred.load();
	Shaders[SHADER_DEFERRED] = shader_deferred;
	ResourceManager::storeShader(SHADER_DEFERRED, shader_deferred);

	Shader shader_deferred_plight_nos;
	shader_deferred_plight_nos.layers = {
		{ Loader::GetPath("/Shaders/texture.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Deferred/deferred_plight_nos.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/Light/plight.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shader_deferred_plight_nos.load();
	Shaders[SHADER_DEFFERED_PLIGHT_NOS] = shader_deferred_plight_nos;
	ResourceManager::storeShader(SHADER_DEFFERED_PLIGHT_NOS, shader_deferred_plight_nos);

	Shader shader_deferred_slight_nos;
	shader_deferred_slight_nos.layers = {
		{ Loader::GetPath("/Shaders/texture.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Deferred/deferred_slight_nos.fragment").c_str(), ShaderType::FRAGMENT },
		{ Loader::GetPath("/Shaders/Light/slight.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shader_deferred_slight_nos.load();
	Shaders[SHADER_DEFFERED_SLIGHT_NOS] = shader_deferred_slight_nos;
	ResourceManager::storeShader(SHADER_DEFFERED_SLIGHT_NOS, shader_deferred_slight_nos);

	Shader shader_font;
	shader_font.layers = {
		{ Loader::GetPath("/Shaders/font.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/font.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_font.load();
	Shaders[SHADER_FONT] = shader_font;
	ResourceManager::storeShader(SHADER_FONT, shader_font);

	Shader shader_skybox;
	shader_skybox.layers = {
		{ Loader::GetPath("/Shaders/skybox.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/skybox.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_skybox.load();
	Shaders[SHADER_SKYBOX] = shader_skybox;
	ResourceManager::storeShader(SHADER_SKYBOX, shader_skybox);

	Shader shader_deferred_geometry;
	shader_deferred_geometry.layers = {
		{ Loader::GetPath("/Shaders/Geometry/geometry.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Geometry/geometry.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_deferred_geometry.load();
	Shaders[SHADER_DEFFERED_GEOMETRY] = shader_deferred_geometry;
	ResourceManager::storeShader(SHADER_DEFFERED_GEOMETRY, shader_deferred_geometry);

	Shader shader_texture;
	shader_texture.layers = {
		{ Loader::GetPath("/Shaders/texture.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/texture.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_texture.load();
	Shaders[SHADER_TEXTURE] = shader_texture;
	ResourceManager::storeShader(SHADER_TEXTURE, shader_texture);

	Shader shader_perspective_depth;
	shader_perspective_depth.layers = {
		{ Loader::GetPath("/Shaders/Depth/depth_persp.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Depth/depth_persp.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_perspective_depth.load();
	Shaders[SHADER_DEPTH_PERSP] = shader_perspective_depth;
	ResourceManager::storeShader(SHADER_DEPTH_PERSP, shader_perspective_depth);

	Shader shader_filter_blur;
	shader_filter_blur.layers = {
		{ Loader::GetPath("/Shaders/texture.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Filter/blur.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_filter_blur.load();
	Shaders[SHADER_FILTER_BLUR] = shader_filter_blur;
	ResourceManager::storeShader(SHADER_FILTER_BLUR, shader_filter_blur);

	Shader shader_instancing_basic;
	shader_instancing_basic.layers = {
		{ Loader::GetPath("/Shaders/Instancing/instancing_basic.vertex").c_str(), ShaderType::VERTEX },
		{ Loader::GetPath("/Shaders/Instancing/instancing_basic.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_instancing_basic.load();
	Shaders[SHADER_INSTANCING_BASIC] = shader_instancing_basic;
	ResourceManager::storeShader(SHADER_INSTANCING_BASIC, shader_instancing_basic);
}

void clearScreen(glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

