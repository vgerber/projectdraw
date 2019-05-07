#include "core.h"


void initCore() {

	Configurator::load();
	Log::load(Configurator::get(Configurator::ProjectLogDir));

	GLenum err = glewInit();

	if (GLEW_OK != err) {
		Log::write(LogType::Error, "Init failed - " + std::string((char*)glewGetErrorString(err)), "Init");
	}
/*
	//depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);



	glEnable(GL_CULL_FACE);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_MULTISAMPLE);

	//glEnable(GL_FRAMEBUFFER_SRGB);


	//glEnable(GL_LINE_SMOOTH);

	//glEnable(GL_LINE_STIPPLE);	
	//glLineStipple(1, 0xAAAA);
*/

	loadDeferredShaders();

	loadForwardShaders();

	loadFlatShaders();

	loadDepthShaders();
	
	loadTextureShaders();

	loadPostprocessingShaders();

	loadExperimentalShaders();

}

void loadDeferredShaders()
{
	Log::write(LogType::Info, "load Deferred shaders", "Init");

	//Debug
	Shader shader_normal_geometry;
	shader_normal_geometry.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Debug/normal.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Debug/normal.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Debug/normal.geometry").c_str(), ShaderType::GEOMETRY }
	};
	shader_normal_geometry.load();
	ResourceManager::storeShader(ShaderName::Renderer::Deferred::Debug::Normal, shader_normal_geometry);

	//Mesh
	Shader shaderBasic;
	shaderBasic.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Mesh/basic.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Mesh/basic.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderBasic.load();
	ResourceManager::storeShader(ShaderName::Renderer::Deferred::Mesh::Basic, shaderBasic);

	Shader shader_font;
	shader_font.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Mesh/font.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Mesh/font.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_font.load();
	ResourceManager::storeShader(ShaderName::Renderer::Deferred::Mesh::Font, shader_font);

	Shader shader_instancing_basic;
	shader_instancing_basic.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Mesh/instancing_basic.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Mesh/instancing_basic.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_instancing_basic.load();
	ResourceManager::storeShader(ShaderName::Renderer::Deferred::Mesh::Instancing, shader_instancing_basic);

	Shader shaderLight;
	shaderLight.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Mesh/light.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Mesh/light.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderLight.load();
	ResourceManager::storeShader(ShaderName::Renderer::Deferred::Mesh::Light, shaderLight);




	//Pipeline

	//Shadow
	Shader shaderPipelineShadowD;
	shaderPipelineShadowD.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Pipeline/pipeline.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Pipeline/Shadow/dlight_pass.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Light/Shadow/dlight_shadow.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shaderPipelineShadowD.load();
	ResourceManager::storeShader(ShaderName::Renderer::Deferred::Pipeline::Shadow::D, shaderPipelineShadowD);

	Shader shaderDeferredPShadow;
	shaderDeferredPShadow.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Pipeline/pipeline.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Pipeline/Shadow/plight_pass.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Light/Shadow/plight_shadow.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shaderDeferredPShadow.load();
	ResourceManager::storeShader(ShaderName::Renderer::Deferred::Pipeline::Shadow::P, shaderDeferredPShadow);

	Shader shaderDeferredSShadow;
	shaderDeferredSShadow.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Pipeline/pipeline.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Pipeline/Shadow/slight_pass.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Light/Shadow/slight_shadow.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shaderDeferredSShadow.load();
	ResourceManager::storeShader(ShaderName::Renderer::Deferred::Pipeline::Shadow::S, shaderDeferredSShadow);



	//No Shadow

	Shader shader_deferred_plight_nos;
	shader_deferred_plight_nos.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Pipeline/pipeline.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Pipeline/plight_pass.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Light/plight.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shader_deferred_plight_nos.load();
	ResourceManager::storeShader(ShaderName::Renderer::Deferred::Pipeline::P, shader_deferred_plight_nos);

	Shader shader_deferred_slight_nos;
	shader_deferred_slight_nos.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Pipeline/pipeline.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Pipeline/slight_pass.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Light/slight.fragment").c_str(), ShaderType::FRAGMENT },
	};
	shader_deferred_slight_nos.load();
	ResourceManager::storeShader(ShaderName::Renderer::Deferred::Pipeline::S, shader_deferred_slight_nos);

	//Texture
	Shader shaderPipelineScreenTexture;
	shaderPipelineScreenTexture.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Pipeline/Texture/texture.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Deferred/Pipeline/Texture/texture.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderPipelineScreenTexture.load();
	ResourceManager::storeShader(ShaderName::Renderer::Deferred::Pipeline::Texture::ScreenTexture, shaderPipelineScreenTexture);
}

void loadForwardShaders() {
	Log::write(LogType::Info, "load Forward shaders", "Init");

	Shader shaderBasicMesh;
	shaderBasicMesh.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/Basic/mesh.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/Basic/mesh.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderBasicMesh.load();
	ResourceManager::storeShader(ShaderName::Renderer::Forward::Basic::Mesh, shaderBasicMesh);

	Shader shaderBasicInstancingPoint2D;
	shaderBasicInstancingPoint2D.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/Basic/Instancing/points2D.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/Basic/Instancing/points2D.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderBasicInstancingPoint2D.load();
	ResourceManager::storeShader(ShaderName::Renderer::Forward::Basic::Instancing::Point2D, shaderBasicInstancingPoint2D);

	Shader shaderBasicInstancingPoint3D;
	shaderBasicInstancingPoint3D.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/Basic/Instancing/points3D.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/Basic/Instancing/points3D.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderBasicInstancingPoint3D.load();
	ResourceManager::storeShader(ShaderName::Renderer::Forward::Basic::Instancing::Point3D, shaderBasicInstancingPoint3D);	

	Shader shaderBasicInstancingBillboard2D;
	shaderBasicInstancingBillboard2D.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/Basic/Instancing/billboard2D.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/Basic/Instancing/billboard2D.geometry").c_str(), ShaderType::GEOMETRY },
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/Basic/Instancing/billboard2D.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderBasicInstancingBillboard2D.load();
	ResourceManager::storeShader(ShaderName::Renderer::Forward::Basic::Instancing::Billboard2D, shaderBasicInstancingBillboard2D);

	Shader shaderBasicInstancingBillboard3D;
	shaderBasicInstancingBillboard3D.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/Basic/Instancing/billboard3D.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/Basic/Instancing/billboard3D.geometry").c_str(), ShaderType::GEOMETRY },
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/Basic/Instancing/billboard3D.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderBasicInstancingBillboard3D.load();
	ResourceManager::storeShader(ShaderName::Renderer::Forward::Basic::Instancing::Billboard3D, shaderBasicInstancingBillboard3D);

	Shader shaderDepthInfoMesh;
	shaderDepthInfoMesh.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/DepthInfo/depthinfo_mesh.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Forward/DepthInfo/depthinfo_mesh.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderDepthInfoMesh.load();
	ResourceManager::storeShader(ShaderName::Renderer::Forward::DepthInfo::Mesh, shaderDepthInfoMesh);


}

void loadDepthShaders()
{
	Log::write(LogType::Info, "load Depth shaders", "Init");

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
	Log::write(LogType::Info, "load Texture shaders", "Init");

	Shader shader_texture;
	shader_texture.layers = {
		{ ResourceManager::GetPath("/Shaders/Texture/texture.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Texture/texture.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shader_texture.load();
	ResourceManager::storeShader(ShaderName::Texture::Basic, shader_texture);
}

void loadPostprocessingShaders() {
	Log::write(LogType::Info, "load Postprocessing shaders", "Init");
	//Antialias
	Shader shaderAntialiasFXAA;
	shaderAntialiasFXAA.layers = {
		{ ResourceManager::GetPath("/Shaders/Postprocessing/texture.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/fxaa.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderAntialiasFXAA.load();
	ResourceManager::storeShader(ShaderName::Postprocessing::Antialias::FXAA, shaderAntialiasFXAA);

	Shader shaderSMAAEdgeLuma;
	shaderSMAAEdgeLuma.layers = {
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/SMAA/smaa_edge.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/SMAA/smaa_functions.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/SMAA/smaa_edge.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/SMAA/smaa_functions.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderSMAAEdgeLuma.load();
	ResourceManager::storeShader(ShaderName::Postprocessing::Antialias::SMAA::EdgeLuma, shaderSMAAEdgeLuma);

	Shader shaderSMAABlendingWeight;
	shaderSMAABlendingWeight.layers = {
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/SMAA/smaa_weight.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/SMAA/smaa_functions.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/SMAA/smaa_weight.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/SMAA/smaa_functions.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderSMAABlendingWeight.load();
	ResourceManager::storeShader(ShaderName::Postprocessing::Antialias::SMAA::BlendingWeight, shaderSMAABlendingWeight);

	Shader shaderSMAABlending;
	shaderSMAABlending.layers = {
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/SMAA/smaa_blend.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/SMAA/smaa_functions.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/SMAA/smaa_blend.fragment").c_str(), ShaderType::FRAGMENT },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/Antialias/SMAA/smaa_functions.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderSMAABlending.load();
	ResourceManager::storeShader(ShaderName::Postprocessing::Antialias::SMAA::Blending, shaderSMAABlending);

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

	Shader shaderSSAO;
	shaderSSAO.layers = {
		{ ResourceManager::GetPath("/Shaders/Postprocessing/texture.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/SSAO/ssao.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderSSAO.load();
	ResourceManager::storeShader(ShaderName::Postprocessing::SSAO::SSAO, shaderSSAO);

	Shader shaderSSAOBlur;
	shaderSSAOBlur.layers = {
		{ ResourceManager::GetPath("/Shaders/Postprocessing/texture.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Postprocessing/SSAO/ssao_blur.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderSSAOBlur.load();
	ResourceManager::storeShader(ShaderName::Postprocessing::SSAO::SSAOBlur, shaderSSAOBlur);

}

void loadFlatShaders()
{
	Log::write(LogType::Info, "load UI shaders", "Init");
	Shader shaderFlatMesh;
	shaderFlatMesh.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Flat/mesh.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Flat/mesh.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderFlatMesh.load();
	ResourceManager::storeShader(ShaderName::Renderer::Flat::Mesh, shaderFlatMesh);

	Shader shaderFlatInstancingPoints;
	shaderFlatInstancingPoints.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Flat/Instancing/points2D.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Flat/Instancing/points2D.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderFlatInstancingPoints.load();
	ResourceManager::storeShader(ShaderName::Renderer::Flat::Instancing::Point, shaderFlatInstancingPoints);

	Shader shaderFlatInstancingBillboard;
	shaderFlatInstancingBillboard.layers = {
		{ ResourceManager::GetPath("/Shaders/Renderer/Flat/Instancing/billboard2D.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Renderer/Flat/Instancing/billboard2D.geometry").c_str(), ShaderType::GEOMETRY },
		{ ResourceManager::GetPath("/Shaders/Renderer/Flat/Instancing/billboard2D.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderFlatInstancingBillboard.load();
	ResourceManager::storeShader(ShaderName::Renderer::Flat::Instancing::Billboard, shaderFlatInstancingBillboard);
}

void loadExperimentalShaders() {
	Log::write(LogType::Info, "load Experimental shaders", "Init");
	Shader shaderOpenglTestBasic;
	shaderOpenglTestBasic.layers = {
		{ ResourceManager::GetPath("/Shaders/Experimental/OpenglTest/basic.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Experimental/OpenglTest/basic.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderOpenglTestBasic.load();
	ResourceManager::storeShader(ShaderName::Experimental::OpenglTest::Basic, shaderOpenglTestBasic);

	Shader shaderOpenglTestBasicPostProcessing;
	shaderOpenglTestBasicPostProcessing.layers = {
		{ ResourceManager::GetPath("/Shaders/Experimental/OpenglTest/basic_postprocessing.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Experimental/OpenglTest/basic_postprocessing.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderOpenglTestBasicPostProcessing.load();
	ResourceManager::storeShader(ShaderName::Experimental::OpenglTest::BasicPostProcessing, shaderOpenglTestBasicPostProcessing);

	Shader shaderOpenglTestFXAA;
	shaderOpenglTestFXAA.layers = {
		{ ResourceManager::GetPath("/Shaders/Experimental/OpenglTest/fxaa.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Experimental/OpenglTest/fxaa.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderOpenglTestFXAA.load();
	ResourceManager::storeShader(ShaderName::Experimental::OpenglTest::FXAA, shaderOpenglTestFXAA);

	Shader shaderOpenglTestBasicTexture;
	shaderOpenglTestBasicTexture.layers = {
		{ ResourceManager::GetPath("/Shaders/Experimental/OpenglTest/texture.vertex").c_str(), ShaderType::VERTEX },
		{ ResourceManager::GetPath("/Shaders/Experimental/OpenglTest/texture.fragment").c_str(), ShaderType::FRAGMENT }
	};
	shaderOpenglTestBasicTexture.load();
	ResourceManager::storeShader(ShaderName::Experimental::OpenglTest::Texture, shaderOpenglTestBasicTexture);
}

void clearScreen(glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

