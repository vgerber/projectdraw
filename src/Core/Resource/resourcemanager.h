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
	namespace Deferred {
		namespace Debug {
			const std::string Normal = "DeferredDebugNormal";
		}
		namespace Mesh {
			const std::string Basic      = "DeferredMeshBasic";
			const std::string Font       = "DeferredMeshFont";
			const std::string Instancing = "DeferredMeshInstancing";
			const std::string Light      = "DeferredMeshLight";
		}
		namespace Pipeline {
			const std::string D = "DeferredPipelineD";
			const std::string P = "DeferredPipelineP";
			const std::string S = "DeferredPipelineS";

			namespace Texture {
				const std::string ScreenTexture = "DeferredPipelineTexture";
			}

			namespace Shadow {
				const std::string D = "DeferredPipelineShadowD";
				const std::string P = "DeferredPipelineShadowP";
				const std::string S = "DeferredPipelineShadowS";
			}
		}		
	}

	namespace Postprocessing {
		namespace Antialias {
			const std::string FXAA = "PostprocessingAntialiasFXAA";
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
		}
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