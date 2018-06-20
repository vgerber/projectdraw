cmake_minimum_required (VERSION 2.6)
project (ProjectDraw)

file(GLOB_RECURSE ENGINE_SRC 
    "src/*.h"
    "src/*.cpp"
)

if (UNIX)
add_executable(engine src/main.cpp ${ENGINE_SRC})

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
find_package(Bullet REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${SOIL_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS})
include_directories(~/Development/Lib/Cpp/reactphysics3d-0.7.0/src)

target_link_libraries(engine assimp glfw ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SOIL_LIBRARIES} ${FREETYPE_LIBRARIES} Bullet3Common BulletSoftBody BulletDynamics BulletCollision BulletInverseDynamicsUtils BulletInverseDynamics LinearMath)
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
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/reactphysics3d-0.7.0/src)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/src)

link_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/glew-2.0.0/lib/Release/Win32)
link_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/glfw-3.2.1/lib-vc2015)
link_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/)
link_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/assimp-4.1.0/build/code/Debug)
link_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/SOIL/projects/VC9/Debug)
link_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/freetype-2.9.1/objs/Win32/Debug)
link_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/reactphysics3d-0.7.0/build/lib/Debug)
link_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Debug)

add_executable(engine src/main.cpp ${ENGINE_SRC})
target_link_libraries(engine 
	assimp-vc140-mt 
	glfw3 
	glew32s 
	SOIL
	OpenGL32 
	freetype 
	BulletSoftBody_Debug
	BulletInverseDynamics_Debug
	BulletInverseDynamicsUtils_Debug
	BulletDynamics_Debug
	BulletCollision_Debug
	Bullet3Geometry_Debug
	Bullet3Common_Debug
	Bullet3Dynamics_Debug
	Bullet3Collision_Debug
	LinearMath_Debug
)
endif()



