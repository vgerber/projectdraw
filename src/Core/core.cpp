#include "core.h"


void initCore() {
	GLenum err = glewInit();

	if (GLEW_OK != err) {
		printf("[CORE] [ERROR] Init failed :: %s", glewGetErrorString(err));
	}

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


	loadDeferredShaders();

	loadDepthShaders();
	
	loadTextureShaders();

	loadPostprocessing();

}

void loadDeferredShaders()
{
	printf("[Shader] load DEFERRED shaders\n");

	//Debug
	Shader shader_normal_geometry;
	shader_normal_geometry.layers = {
		{ ResourceManager::GetPath("/Shaders/Deferred/Debug/normal.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Deferred/Debug/normal.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Deferred/Debug/normal.geometry").c_str(), ShaderType::GEOMETRY }
	};
	shader_normal_geometry.load();
	ResourceManager::storeShader(ShaderName::Deferred::Debug::Normal, shader_normal_geometry);



	//Mesh
	Shader shaderBasic;
	shaderBasic.layers = {
		{ ResourceManager::GetPath("/Shaders/Deferred/Mesh/basic.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Deferred/Mesh/basic.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderBasic.load();
	ResourceManager::storeShader(ShaderName::Deferred::Mesh::Basic, shaderBasic);

	Shader shader_font;
	shader_font.layers = {
		{ ResourceManager::GetPath("/Shaders/Deferred/Mesh/font.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Deferred/Mesh/font.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_font.load();
	ResourceManager::storeShader(ShaderName::Deferred::Mesh::Font, shader_font);

	Shader shader_instancing_basic;
	shader_instancing_basic.layers = {
		{ ResourceManager::GetPath("/Shaders/Deferred/Mesh/instancing_basic.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Deferred/Mesh/instancing_basic.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_instancing_basic.load();
	ResourceManager::storeShader(ShaderName::Deferred::Mesh::Instancing, shader_instancing_basic);

	Shader shaderLight;
	shaderLight.layers = {
		{ ResourceManager::GetPath("/Shaders/Deferred/Mesh/light.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Deferred/Mesh/light.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderLight.load();
	ResourceManager::storeShader(ShaderName::Deferred::Mesh::Light, shaderLight);




	//Pipeline

	//Shadow
	Shader shaderPipelineShadowD;
	shaderPipelineShadowD.layers = {
		{ ResourceManager::GetPath("/Shaders/Deferred/Pipeline/pipeline.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Deferred/Pipeline/Shadow/dlight_pass.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Deferred/Light/Shadow/dlight_shadow.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shaderPipelineShadowD.load();
	ResourceManager::storeShader(ShaderName::Deferred::Pipeline::Shadow::D, shaderPipelineShadowD);

	Shader shaderDeferredPShadow;
	shaderDeferredPShadow.layers = {
		{ ResourceManager::GetPath("/Shaders/Deferred/Pipeline/pipeline.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Deferred/Pipeline/Shadow/plight_pass.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Deferred/Light/Shadow/plight_shadow.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shaderDeferredPShadow.load();
	ResourceManager::storeShader(ShaderName::Deferred::Pipeline::Shadow::P, shaderDeferredPShadow);

	Shader shaderDeferredSShadow;
	shaderDeferredSShadow.layers = {
		{ ResourceManager::GetPath("/Shaders/Deferred/Pipeline/pipeline.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Deferred/Pipeline/Shadow/slight_pass.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Deferred/Light/Shadow/slight_shadow.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shaderDeferredSShadow.load();
	ResourceManager::storeShader(ShaderName::Deferred::Pipeline::Shadow::S, shaderDeferredSShadow);



	//No Shadow

	Shader shader_deferred_plight_nos;
	shader_deferred_plight_nos.layers = {
		{ ResourceManager::GetPath("/Shaders/Deferred/Pipeline/pipeline.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Deferred/Pipeline/plight_pass.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Deferred/Light/plight.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shader_deferred_plight_nos.load();
	ResourceManager::storeShader(ShaderName::Deferred::Pipeline::P, shader_deferred_plight_nos);

	Shader shader_deferred_slight_nos;
	shader_deferred_slight_nos.layers = {
		{ ResourceManager::GetPath("/Shaders/Deferred/Pipeline/pipeline.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Deferred/Pipeline/slight_pass.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Deferred/Light/slight.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shader_deferred_slight_nos.load();
	ResourceManager::storeShader(ShaderName::Deferred::Pipeline::S, shader_deferred_slight_nos);

	//Texture
	Shader shaderPipelineScreenTexture;
	shaderPipelineScreenTexture.layers = {
		{ ResourceManager::GetPath("/Shaders/Deferred/Pipeline/Texture/texture.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Deferred/Pipeline/Texture/texture.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderPipelineScreenTexture.load();
	ResourceManager::storeShader(ShaderName::Deferred::Pipeline::Texture::ScreenTexture, shaderPipelineScreenTexture);
}

void loadDepthShaders()
{
	printf("\n[Shader] load DEPTH shaders\n");

	Shader shader_perspective_depth;
	shader_perspective_depth.layers = {
		{ ResourceManager::GetPath("/Shaders/Depth/depth_persp.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Depth/depth_persp.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_perspective_depth.load();
	ResourceManager::storeShader(ShaderName::Depth::Persp, shader_perspective_depth);

	Shader shader_depth_cube;
	shader_depth_cube.layers = {
		{ ResourceManager::GetPath("/Shaders/Depth/cube_depth.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Depth/cube_depth.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Depth/cube_depth.geometry").c_str(), ShaderType::GEOMETRY }
	};
	shader_depth_cube.load();
	ResourceManager::storeShader(ShaderName::Depth::PerspCube, shader_depth_cube);

	Shader shader_depth;
	shader_depth.layers = {
		{ ResourceManager::GetPath("/Shaders/Depth/depth.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Depth/depth.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_depth.load();
	ResourceManager::storeShader(ShaderName::Depth::Ortho, shader_depth);
}

void loadTextureShaders()
{
	printf("\n[Shader] load TEXTURE shaders\n");

	Shader shader_texture;
	shader_texture.layers = {
		{ ResourceManager::GetPath("/Shaders/Texture/texture.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Texture/texture.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_texture.load();
	ResourceManager::storeShader(ShaderName::Texture::Basic, shader_texture);
}

void loadPostprocessing() {
	printf("\n[Shader] load POSTPROCESSING shaders\n");
	//Antialias
	Shader shaderAntialiasFXAA;
	shaderAntialiasFXAA.layers = {
		{ ResourceManager::GetPath("/Shaders/Postprocessing/texture.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/fxaa.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderAntialiasFXAA.load();
	ResourceManager::storeShader(ShaderName::Postprocessing::Antialias::FXAA, shaderAntialiasFXAA);

	//HDR
	Shader shaderHDRBasic;
	shaderHDRBasic.layers = {
		{ ResourceManager::GetPath("/Shaders/Postprocessing/texture.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/HDR/hdr.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderHDRBasic.load();
	ResourceManager::storeShader(ShaderName::Postprocessing::HDR::Basic, shaderHDRBasic);

	//Bloom
	Shader shaderBloomBlur;
	shaderBloomBlur.layers = {
		{ ResourceManager::GetPath("/Shaders/Postprocessing/texture.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Bloom/blur.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderBloomBlur.load();
	ResourceManager::storeShader(ShaderName::Postprocessing::Bloom::Blur, shaderBloomBlur);

	Shader shaderBloomMerge;
	shaderBloomMerge.layers = {
		{ ResourceManager::GetPath("/Shaders/Postprocessing/texture.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Bloom/merge.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderBloomMerge.load();
	ResourceManager::storeShader(ShaderName::Postprocessing::Bloom::Merge, shaderBloomMerge);

}


void clearScreen(glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

