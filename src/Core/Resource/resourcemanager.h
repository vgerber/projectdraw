#pragma once

#include <map>

#include "Core/Shader/shader.h"
#include "Core/Mesh/Primitives/basicmesh.h"


#ifdef _WIN32
const std::string ROOT_DIR = "C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/src";
#endif

#ifdef linux
const std::string ROOT_DIR = "/home/vincent/Development/Cpp/projectdraw_slim/src";
#endif


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
			}
		}

		namespace Flat {
			const std::string Mesh = "RendererRendererFlatMesh";
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
		const std::string VeraMono = "BaseVeraMono";
	}
}

class ResourceManager {
public:
	static std::string GetPath(const GLchar* path);

	static void storeShader(std::string name, Shader shader);
	static Shader loadShader(std::string name);
	static void deleteShader(std::string name);

	static void free();
private:
	static std::map<std::string, Shader> shaders;


};