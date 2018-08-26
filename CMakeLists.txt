cmake_minimum_required (VERSION 2.8)
project (ProjectDraw)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

file(GLOB_RECURSE ENGINE_SRC 
    "src/*.h"
    "src/*.cpp"
)

if (UNIX)
add_library(projectdraw ${ENGINE_SRC})

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
find_package(SFML 2.5 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${SOIL_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} 
	#${BULLET_INCLUDE_DIRS}
	/home/vincent/Development/Lib/Cpp/bullet3/src/
)


target_link_libraries(projectdraw
#SFML
	sfml-graphics
	sfml-window
	sfml-system
#Assimp
	debug 	  /home/vincent/Development/Lib/Cpp/assimp-master/build/code/libassimp.so 
	optimized /home/vincent/Development/Lib/Cpp/assimp-master/Release/code/libassimp.so 
#GLFW
	glfw 

	${OPENGL_LIBRARIES} 
	${GLEW_LIBRARIES} 
	${SOIL_LIBRARIES} 
	${FREETYPE_LIBRARIES} 
#Bullet
	debug /home/vincent/Development/Lib/Cpp/bullet3/build/src/Bullet3Common/libBullet3Common.so
	debug /home/vincent/Development/Lib/Cpp/bullet3/build/src/BulletSoftBody/libBulletSoftBody.so
	debug /home/vincent/Development/Lib/Cpp/bullet3/build/src/BulletDynamics/libBulletDynamics.so
	debug /home/vincent/Development/Lib/Cpp/bullet3/build/src/BulletCollision/libBulletCollision.so
	debug /home/vincent/Development/Lib/Cpp/bullet3/build/src/BulletInverseDynamics/libBulletInverseDynamics.so
	debug /home/vincent/Development/Lib/Cpp/bullet3/build/src/LinearMath/libLinearMath.so

	optimized /home/vincent/Development/Lib/Cpp/bullet3/Release/src/Bullet3Common/libBullet3Common.so
	optimized /home/vincent/Development/Lib/Cpp/bullet3/Release/src/BulletSoftBody/libBulletSoftBody.so
	optimized /home/vincent/Development/Lib/Cpp/bullet3/Release/src/BulletDynamics/libBulletDynamics.so
	optimized /home/vincent/Development/Lib/Cpp/bullet3/Release/src/BulletCollision/libBulletCollision.so
	optimized /home/vincent/Development/Lib/Cpp/bullet3/Release/src/BulletInverseDynamics/libBulletInverseDynamics.so
	optimized /home/vincent/Development/Lib/Cpp/bullet3/Release/src/LinearMath/libLinearMath.so
)
endif()

if(WIN32)


#add_subdirectory(C:/Users/Vincent/Documents/Projects/Lib/C++)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/glfw-3.2.1/include)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/SFML-2.5.0/include)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/glew-2.0.0/include)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/glm)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/assimp-4.1.0/include)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/assimp-4.1.0/build/include)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/SOIL/src)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/freetype-2.9.1/include)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/src)



#add_executable(engine src/main.cpp ${ENGINE_SRC})
add_library(projectdraw STATIC  ${ENGINE_SRC})
target_link_libraries(projectdraw
#ASSIMP
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/C++/assimp-4.1.0/build/code/Debug/assimp-vc140-mt.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/assimp-4.1.0/build/code/Release/assimp-vc140-mt.lib
#GLEW
	C:/Users/Vincent/Documents/Projects/Lib/C++/glew-2.0.0/lib/Release/Win32/glew32s.lib 
#SFML
	debug     C:/Users/Vincent/Documents/Projects/Lib/C++/SFML-2.5.0/lib/sfml-graphics-d.lib
	debug     C:/Users/Vincent/Documents/Projects/Lib/C++/SFML-2.5.0/lib/sfml-window-d.lib
	debug     C:/Users/Vincent/Documents/Projects/Lib/C++/SFML-2.5.0/lib/sfml-system-d.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/SFML-2.5.0/lib/sfml-graphics.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/SFML-2.5.0/lib/sfml-window.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/SFML-2.5.0/lib/sfml-system.lib
#SOIL
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/C++/SOIL/projects/VC9/Debug/SOIL.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/SOIL/projects/VC9/Release/SOIL.lib	
#OpenGL
	OpenGL32 
#FreeType
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/C++/freetype-2.9.1/objs/Win32/Debug/freetype.lib 
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/freetype-2.9.1/objs/Win32/Release/freetype.lib 
#Bullet
	debug C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Debug/BulletSoftBody_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Debug/BulletInverseDynamics_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Debug/BulletInverseDynamicsUtils_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Debug/BulletDynamics_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Debug/BulletCollision_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Debug/Bullet3Geometry_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Debug/Bullet3Common_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Debug/Bullet3Dynamics_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Debug/Bullet3Collision_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Debug/LinearMath_Debug.lib

	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Release/BulletSoftBody.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Release/BulletInverseDynamics.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Release/BulletInverseDynamicsUtils.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Release/BulletDynamics.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Release/BulletCollision.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Release/Bullet3Geometry.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Release/Bullet3Common.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Release/Bullet3Dynamics.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Release/Bullet3Collision.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/build/lib/Release/LinearMath.lib
)
endif()



