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

file(GLOB_RECURSE PHYSICS_SRC
	"src/Physics/*.h"
	"src/Physics/*.cpp"
)

file(GLOB_RECURSE EXAMPLE_3D_SRC
	"examples/3d_demo/*.h"
	"examples/3d_demo/*.cpp"
)

file(GLOB_RECURSE EXAMPLE_PHYSICS3D_SRC
	"examples/physics3d_example/*.h"
	"examples/physics3d_example/*.cpp"
)

file(GLOB_RECURSE EXPERIMENTAL_OPENGL
	"test/opengl_test/*.h"
	"test/opengl_test/*.cpp"
)

if (UNIX)

set(BulletLibraries
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

find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

add_library(projectdraw_core ${ENGINE_SRC}) 
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${SOIL_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS}
	/home/vincent/Development/Lib/Cpp/stb-master	
	/home/vincent/Development/Lib/Cpp/glm-0.9.9.0/
)
target_link_libraries(projectdraw_core
#Assimp
	debug 	  /home/vincent/Development/Lib/Cpp/assimp-master/build/code/libassimp.so 
	optimized /home/vincent/Development/Lib/Cpp/assimp-master/Release/code/libassimp.so 

	${OPENGL_LIBRARIES} 
	${GLEW_LIBRARIES} 
	${SOIL_LIBRARIES} 
	${FREETYPE_LIBRARIES} 
)

add_library(projectdraw_physics3d ${PHYSICS_SRC})
include_directories(
	/home/vincent/Development/Lib/Cpp/bullet3/src/
	/home/vincent/Development/Cpp/projectdraw_slim/src/
)

target_link_libraries(projectdraw_physics3d
	${BulletLibraries}
	/home/vincent/Development/Cpp/projectdraw_slim/build/projectdraw_core.a
)



#Examples


#Scene3D Example
include_directories(
	/home/vincent/Development/Lib/Cpp/glm-0.9.9.0/
	/home/vincent/Development/Cpp/projectdraw_slim/src/
	/home/vincent/Development/Lib/Cpp/stb-master/
	${FREETYPE_INCLUDE_DIRS} 
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
)

add_executable(3d_example ${EXAMPLE_3D_SRC})

target_link_libraries(3d_example
	sfml-graphics
	sfml-window
	sfml-system
	/home/vincent/Development/Cpp/projectdraw_slim/build/libprojectdraw_core.a
	${FREETYPE_LIBRARIES} 
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES} 

	debug 	  /home/vincent/Development/Lib/Cpp/assimp-master/build/code/libassimp.so 
	optimized /home/vincent/Development/Lib/Cpp/assimp-master/Release/code/libassimp.so 
)


#Scene3D Example
include_directories(
	/home/vincent/Development/Lib/Cpp/glm-0.9.9.0/
	/home/vincent/Development/Cpp/projectdraw_slim/src/
	/home/vincent/Development/Lib/Cpp/stb-master/
	${FREETYPE_INCLUDE_DIRS} 
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
)

add_executable(physics3d_example ${EXAMPLE_PHYSICS3D_SRC})

target_link_libraries(physics3d_example
	sfml-graphics
	sfml-window
	sfml-system
	/home/vincent/Development/Cpp/projectdraw_slim/build/libprojectdraw_core.a
	/home/vincent/Development/Cpp/projectdraw_slim/build/libprojectdraw_physics3d.a
	${FREETYPE_LIBRARIES} 
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES} 
	${BulletLibraries}

	debug 	  /home/vincent/Development/Lib/Cpp/assimp-master/build/code/libassimp.so 
	optimized /home/vincent/Development/Lib/Cpp/assimp-master/Release/code/libassimp.so 
)

#Experimental
add_executable(opengl_test ${EXPERIMENTAL_OPENGL})

target_link_libraries(opengl_test
	sfml-graphics
	sfml-window
	sfml-system
	/home/vincent/Development/Cpp/projectdraw_slim/build/libprojectdraw_core.a
	/home/vincent/Development/Cpp/projectdraw_slim/build/libprojectdraw_physics3d.a
	${FREETYPE_LIBRARIES} 
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES} 
	${BulletLibraries}

	debug 	  /home/vincent/Development/Lib/Cpp/assimp-master/build/code/libassimp.so 
	optimized /home/vincent/Development/Lib/Cpp/assimp-master/Release/code/libassimp.so 
)
endif()


if(WIN32)


include_directories(C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/include)
include_directories(C:/Users/Vincent/Documents/Projects/Lib/C++/stb)


set(BulletLibraries 
	debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/BulletSoftBody_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/BulletInverseDynamics_Debug.lib
	#debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/BulletInverseDynamicsUtils_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/BulletDynamics_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/BulletCollision_Debug.lib
	#debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/Bullet3Geometry_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/Bullet3Common_Debug.lib
	#debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/Bullet3Dynamics_Debug.lib
	#debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/Bullet3Collision_Debug.lib
	debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/LinearMath_Debug.lib

	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/BulletSoftBody.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/BulletInverseDynamics.lib
	#optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/BulletInverseDynamicsUtils.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/BulletDynamics.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/BulletCollision.lib
	#optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/Bullet3Geometry.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/Bullet3Common.lib
	#optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/Bullet3Dynamics.lib
	#optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/Bullet3Collision.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/LinearMath.lib
)

set(ProjectDrawLibraries
	debug 	   C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Debug/projectdraw_core.lib
	optimized  C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Release/projectdraw_core.lib
	debug 	   C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Debug/projectdraw_physics3d.lib
	optimized  C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Release/projectdraw_physics3d.lib
)

add_library(projectdraw_core STATIC  ${ENGINE_SRC})

set(ENGINE_ROOT "C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/src/Core/")
foreach(source_item IN ITEMS ${ENGINE_SRC})
    get_filename_component(source_item_path "${source_item}" PATH)
    file(RELATIVE_PATH source_item_rel_path "${ENGINE_ROOT}" "${source_item_path}")
    string(REPLACE "/" "\\" item_group_path "${source_item_rel_path}")
    source_group("${item_group_path}" FILES "${source_item}")
endforeach()

target_link_libraries(projectdraw_core
#ASSIMP
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/assimp-vc140-mtd.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/assimp-vc140-mt.lib
#GLEW
	debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/glew32d.lib 
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/glew32.lib 
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
debug 	   C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Debug/projectdraw_core.lib
optimized  C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Release/projectdraw_core.lib
)

add_library(projectdraw_recorder ${RECORDER_SRC})
include_directories(C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/src)
target_link_libraries(projectdraw_recorder
	debug 	   C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Debug/projectdraw_core.lib
	optimized  C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Release/projectdraw_core.lib
#ffmpeg
	debug C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/debug/avcodec.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/avcodec.lib
)

#Physics Module
add_library(projectdraw_physics3d ${PHYSICS_SRC})
include_directories( 
	C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/src
	C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/src
)
target_link_libraries(projectdraw_physics3d
	debug 	   C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Debug/projectdraw_core.lib
	optimized  C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Release/projectdraw_core.lib
	${BulletLibraries}
)


#Examples


#3D Example
include_directories(C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/include)
include_directories(C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/src)



add_executable(example_scene3d ${EXAMPLE_3D_SRC})
target_link_libraries(example_scene3d
#ASSIMP
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/assimp-vc140-mtd.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/assimp-vc140-mt.lib
#GLEW
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/glew32d.lib 
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/glew32.lib 
#OpenGL
	debug	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/OpenGL32.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/OpenGL32.lib
#FreeType
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/freetyped.lib 
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/freetype.lib 
#SFML
	debug     C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/sfml-graphics-d.lib
	debug     C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/sfml-window-d.lib
	debug     C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/sfml-system-d.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/sfml-graphics.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/sfml-window.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/sfml-system.lib
#ProjectDraw
	${ProjectDrawLibraries}
)


add_executable(example_physics3d ${EXAMPLE_PHYSICS3D_SRC})
target_include_directories(example_physics3d PUBLIC
	C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/src
	C:/Users/Vincent/Documents/Projects/Lib/C++/bullet3-master/src
)
target_link_libraries(example_physics3d
#ASSIMP
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/assimp-vc140-mtd.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/assimp-vc140-mt.lib
#GLEW
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/glew32d.lib 
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/glew32.lib 
#OpenGL
	debug	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/OpenGL32.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/OpenGL32.lib
#FreeType
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/freetyped.lib 
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/freetype.lib 
#SFML
	debug     C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/sfml-graphics-d.lib
	debug     C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/sfml-window-d.lib
	debug     C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/sfml-system-d.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/sfml-graphics.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/sfml-window.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/sfml-system.lib
#ProjectDraw
	${ProjectDrawLibraries}
#Bullet
	${BulletLibraries}
)

endif()



