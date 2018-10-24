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

include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${SOIL_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})


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


#add_subdirectory(C:/Users/Vincent/Documents/Projects/Lib/C++)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/glfw-3.2.1/include)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/glew-2.0.0/include)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/glm)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/assimp-4.1.0/include)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/assimp-4.1.0/build/include)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/SOIL/src)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/freetype-2.9.1/include)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/stb)



#add_executable(engine src/main.cpp ${ENGINE_SRC})
add_library(projectdraw_slim STATIC  ${ENGINE_SRC})
target_link_libraries(projectdraw_slim
#ASSIMP
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/C++/assimp-4.1.0/build/code/Debug/assimp-vc140-mt.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/assimp-4.1.0/build/code/Release/assimp-vc140-mt.lib
#GLEW
	C:/Users/Vincent/Documents/Projects/Lib/C++/glew-2.0.0/lib/Release/Win32/glew32s.lib 
#SOIL
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/C++/SOIL/projects/VC9/Debug/SOIL.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/SOIL/projects/VC9/Release/SOIL.lib	
#OpenGL
	OpenGL32 
#FreeType
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/C++/freetype-2.9.1/objs/Win32/Debug/freetype.lib 
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/freetype-2.9.1/objs/Win32/Release/freetype.lib 
)

add_library(cuda_render STATIC ${CUDARENDER_SRC})
target_link_libraries(cuda_render
	debug 	   C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Debug/projectdraw.lib
	optimized  C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Release/projectdraw.lib
)

endif()



