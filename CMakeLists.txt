cmake_minimum_required (VERSION 2.8)
project (ProjectDraw_Slim)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

file(GLOB_RECURSE ENGINE_SRC 
    "src/Core/*.h"
    "src/Core/*.cpp"	
)

file(GLOB_RECURSE CUDARENDER_SRC
	"src/CudaRender/*.h"
	"src/CudaRender/*.cpp"
)

file(GLOB_RECURSE RECORDER_SRC
	"src/Recorder/*.h"
	"src/Recorder/*.cpp"
)

if (UNIX)
add_library(projectdraw_slim ${ENGINE_SRC})

IF (SOIL_INCLUDE_DIR)
  # Already in cache, be silent
  SET(SOIL_FIND_QUIETLY TRUE)
ENDIF (SOIL_INCLUDE_DIR)


FIND_PATH(SOIL_INCLUDE_DIR SOIL.h PATH_SUFFIXES include/SOIL include)

SET(SOIL_NAMES SOIL Soil soil)
FIND_LIBRARY(SOIL_LIBRARY NAMES ${SOIL_NAMES} )
MARK_AS_ADVANCED( SOIL_LIBRARY SOIL_INCLUDE_DIR )

# Per-recommendation
SET(SOIL_INCLUDE_DIRS "${SOIL_INCLUDE_DIR}")
SET(SOIL_LIBRARIES    "${SOIL_LIBRARY}")

# handle the QUIETLY and REQUIRED arguments and set SOIL_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SOIL DEFAULT_MSG SOIL_LIBRARIES SOIL_INCLUDE_DIRS)    


include_directories(~/Development/Lib/Cpp/glm-0.9.9.0/)

#define GLM_ENABLE_EXPERIMENTAL
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${SOIL_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS}
	/home/vincent/Development/Lib/Cpp/stb-master
)


target_link_libraries(projectdraw_slim
#Assimp
	debug 	  /home/vincent/Development/Lib/Cpp/assimp-master/build/code/libassimp.so 
	optimized /home/vincent/Development/Lib/Cpp/assimp-master/Release/code/libassimp.so 
#GLFW
	glfw 

	${OPENGL_LIBRARIES} 
	${GLEW_LIBRARIES} 
	${SOIL_LIBRARIES} 
	${FREETYPE_LIBRARIES} 
)
endif()

if(WIN32)


include_directories(C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/include)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/stb)



#add_executable(engine src/main.cpp ${ENGINE_SRC})
add_library(projectdraw_slim STATIC  ${ENGINE_SRC})
target_link_libraries(projectdraw_slim
#ASSIMP
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/assimp-vc140-mtd.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/assimp-vc140-mt.lib
#GLEW
	debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/glew32d.lib 
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/glew32.lib 
#SOIL
	#debug 	  C:/Users/Vincent/Documents/Projects/Lib/C++/SOIL/projects/VC9/Debug/SOIL.lib
	#optimized C:/Users/Vincent/Documents/Projects/Lib/C++/SOIL/projects/VC9/Release/SOIL.lib	
#OpenGL
	debug	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/debug/OpenGL32.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/OpenGL32.lib 
#FreeType
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/freetyped.lib 
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/freetype.lib 
)


include_directories(C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/src)
find_package(CUDA QUIET REQUIRED)

cuda_add_library(cuda_render STATIC ${CUDARENDER_SRC})
target_link_libraries(cuda_render
	#C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Cuda/curender.lib
	debug 	   C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Debug/projectdraw.lib
	optimized  C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Release/projectdraw.lib
)

include_directories(C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/src)
add_library(projectdraw_recorder ${RECORDER_SRC})
target_link_libraries(projectdraw_recorder
	debug 	   C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Debug/projectdraw.lib
	optimized  C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Release/projectdraw.lib
#ffmpeg
	debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/debug/avcodec.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/avcodec.lib
)

endif()



