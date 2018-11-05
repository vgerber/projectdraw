cmake_minimum_required (VERSION 2.8)
project (Demo_Cuda)

file(GLOB_RECURSE SRC_FOLDER
	"src/*.h"
	"src/*.cpp"
)

if(WIN32)

find_package(CUDA QUIET REQUIRED)

include_directories(C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/include)
include_directories(C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/src)

cuda_add_executable(cuda_demo ${SRC_FOLDER})
target_link_libraries(cuda_demo
#ASSIMP
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/assimp-vc140-mtd.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/assimp-vc140-mt.lib
#GLEW
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/glew32d.lib 
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/glew32.lib 
#OpenGL
	debug	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/OpenGL32.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/OpenGL32.lib
#SFML
	debug     C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/sfml-graphics-d.lib
	debug     C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/sfml-window-d.lib
	debug     C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/sfml-system-d.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/sfml-graphics.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/sfml-window.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/sfml-system.lib
#ProjectDraw
	debug 	   C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Debug/projectdraw_slim.lib
	optimized  C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Release/projectdraw_slim.lib
	optimized  C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Release/projectdraw_recorder.lib
	debug 	   C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Debug/cuda_render.lib
	optimized  C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Release/cuda_render.lib
	optimized  C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Cuda/curender.lib
#Cuda
	#${CUDA_LIBRARIES}
#Custom Cuda Libs
	C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/examples/cuda_demo/lib/gol.lib


)
endif()
