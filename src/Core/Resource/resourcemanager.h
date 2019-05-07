#pragma once

#include <map>

#include "Core/Shader/shader.h"
#include "Core/Scene/Text/font.h"
#include "Core/Mesh/Primitives/mesh.h"
#include "Core/Util/Config/configurator.h"


namespace ShaderName {

	namespace Renderer {
		namespace Deferred {
			namespace Debug {
				const std::string Normal = "RendererDeferredDebugNormal";
			}
			namespace Mesh {
				const std::string Basic = "RendererDeferredMeshBasic";
				const std::string Font = "RendererDeferredMeshFont";
				const std::string Instancing = "RendererDeferredMeshInstancing";
				const std::string Light = "RendererDeferredMeshLight";
			}
			namespace Pipeline {
				const std::string D = "RendererDeferredPipelineD";
				const std::string P = "RendererDeferredPipelineP";
				const std::string S = "RendererDeferredPipelineS";

				namespace Texture {
					const std::string ScreenTexture = "RendererDeferredPipelineTexture";
				}

				namespace Shadow {
					const std::string D = "RendererDeferredPipelineShadowD";
					const std::string P = "RendererDeferredPipelineShadowP";
					const std::string S = "RendererDeferredPipelineShadowS";
				}
			}
		}

		namespace Forward {
			namespace Basic {
				const std::string Mesh = "RendererForwardBasicMesh";

				namespace Instancing {
					const std::string Point2D = "RendererForwardInstancingPoint2D";
					const std::string Point3D = "RendererForwardInstancingPoint3D";
					const std::string Billboard2D = "RendererForwardInstancingBillboard2D";
					const std::string Billboard3D = "RendererForwardInstancingBillboard3D";
				}
			}

			namespace DepthInfo {
				const std::string Mesh = "RendererForwardDepthInfoMesh";
			}
		}

		namespace Flat {
			const std::string Mesh = "RendererFlatMesh";

			namespace Instancing {
				const std::string Point = "RendererFlatInstancingPoint";
				const std::string Billboard = "RendererFlatInstancingBillboard";
			}
		}
	}

	namespace Postprocessing {
		namespace Antialias {
			const std::string FXAA = "PostprocessingAntialiasFXAA";

			namespace SMAA {
				const std::string EdgeLuma = "ExperimentalOpenglTestSMAAEdgeLuma";
				const std::string BlendingWeight = "ExperimentalOpenglTestSMAABlendingWeight";
				const std::string Blending = "ExperimentalOpenglTestSMAABlending";
			}
		}

		namespace HDR {
			const std::string Basic = "PosprocessingHDRBasic";
		}

		namespace Bloom {
			const std::string Blur = "PostprocessingBloomBlur";
			const std::string Merge = "PostprocessingBloomMerge";
		}

		namespace SSAO {
			const std::string SSAO = "PostprocessingSSAO";
			const std::string SSAOBlur = "PostprocessingSSAOBlur";
		}
	}

	namespace Texture {
		const std::string Basic = "TextureBasic";
	}

	namespace Depth {
		const std::string Persp     = "DepthP";
		const std::string PerspCube = "DepthPC";
		const std::string Ortho     = "DepthO";
	}

	namespace Experimental {
		namespace OpenglTest {
			const std::string Basic = "ExperimentalOpenglTestBasic";
			const std::string BasicPostProcessing = "ExperimentalOpenglTestBasicPostProcessing";
			const std::string FXAA = "ExperimentalOpenglTestFXAA";
			const std::string Texture = "ExperimentalOpenglTestTexture";
		}
	}
}

namespace FontName {
	namespace Base {
		const std::string Arial = "/Fonts/arial.ttf";
	}
}

class ResourceManager {
public:
	/**
	 * @brief Combines relative path in Engine with EngineSrc path
	 * 
	 * @param path Relative path
	 * @return std::string Absolute path to resource in EngineRoot/src
	 */
	static std::string GetPath(std::string path);

	/**
	 * @brief Combines relative path in Project with ProjectSrc üath
	 * 
	 * @param path Relative path
	 * @return std::string Absolute path to resource in ProjectSrc
	 */
	static std::string GetProjectPath(std::string path);

	/**
	 * @brief Stores shader for later use
	 * 
	 * Please check for name collisions
	 * 
	 * @param name Shader storage name
	 * @param shader 
	 */
	static void storeShader(std::string name, Shader shader);

	/**
	 * @brief Get shader from storage
	 * 
	 * @param name 
	 * @return Shader 
	 */
	static Shader loadShader(std::string name);

	/**
	 * @brief Remove shader from storage
	 * 
	 * @param name 
	 */
	static void deleteShader(std::string name);

	/**
	 * @brief Free allocated memory
	 * 
	 */
	static void dispose();
private:
	static std::map<std::string, Shader> shaders;
};