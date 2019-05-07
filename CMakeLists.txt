cmake_minimum_required (VERSION 2.8)
project (ProjectDraw_Slim)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")


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

file(GLOB_RECURSE EXAMPLE_2D_SRC
	"examples/2d_demo/*.h"
	"examples/2d_demo/*.cpp"
)

file(GLOB_RECURSE EXAMPLE_PHYSICS3D_SRC
	"examples/physics3d_example/*.h"
	"examples/physics3d_example/*.cpp"
)

file(GLOB_RECURSE EXPERIMENTAL_OPENGL
	"test/opengl_test/*.h"
	"test/opengl_test/*.cpp"
)


find_package(stb REQUIRED)
find_package(GLM REQUIRED)
find_package(Bullet REQUIRED)
find_package(GLEW REQUIRED)
find_package(Assimp REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(SFML REQUIRED)


add_library(projectdraw_core ${ENGINE_SRC}) 
include_directories(
	${Assimp_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS} 
	${GLEW_INCLUDE_DIRS} 
	${FREETYPE_INCLUDE_DIRS} 
	${stb_INCLUDE_DIRS} 
	${GLM_INCLUDE_DIRS}
)

target_link_libraries(projectdraw_core
	${Assimp_LIBRARIES}
	${OPENGL_LIBRARIES} 
	${GLEW_LIBRARIES} 
	${FREETYPE_LIBRARIES} 
)

add_library(projectdraw_physics3d ${PHYSICS_SRC})
add_dependencies(projectdraw_physics3d projectdraw_core)
include_directories(
	${Bullet_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(projectdraw_physics3d
	debug /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libprojectdraw_core.a
	optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libprojectdraw_core.a
)


if(${SFML_FOUND})
#Examples
	#Scene3D Example
	include_directories(
		${FREETYPE_INCLUDE_DIRS} 
		${OPENGL_INCLUDE_DIRS}
		${GLEW_INCLUDE_DIRS}
		${stb_INCLUDE_DIRS}
		${GLM_INCLUDE_DIRS}
		${CMAKE_SOURCE_DIR}/src
	)

	add_executable(3d_example ${EXAMPLE_3D_SRC})
	add_dependencies(3d_example projectdraw_core)
	target_link_libraries(3d_example
		${FREETYPE_LIBRARIES} 
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARIES} 
		${Assimp_LIBRARIES}
		${SFML_LIBRARIES}
		debug     /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libprojectdraw_core.a
		optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libprojectdraw_core.a
	)
	
	add_executable(2d_example ${EXAMPLE_2D_SRC})
	add_dependencies(2d_example projectdraw_core)
	target_link_libraries(2d_example
		${FREETYPE_LIBRARIES} 
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARIES} 
		${Assimp_LIBRARIES}
		${SFML_LIBRARIES}
		debug     /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libprojectdraw_core.a
		optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libprojectdraw_core.a
	)

	#Physics3D Example
	include_directories(
		${Bullet_INCLUDE_DIRS}
		${FREETYPE_INCLUDE_DIRS} 
		${OPENGL_INCLUDE_DIRS}
		${GLEW_INCLUDE_DIRS}
		${stb_INCLUDE_DIRS}
		${GLM_INCLUDE_DIRS}
		${CMAKE_SOURCE_DIR}/src
	)

	add_executable(physics3d_example ${EXAMPLE_PHYSICS3D_SRC})
	add_dependencies(physics3d_example projectdraw_core projectdraw_physics3d)
	target_link_libraries(physics3d_example
		${FREETYPE_LIBRARIES} 
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARIES} 
		${Assimp_LIBRARIES}
		${SFML_LIBRARIES}
		debug     /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libprojectdraw_core.a
		optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libprojectdraw_core.a
		debug     /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libprojectdraw_physics3d.a
		optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libprojectdraw_physics3d.a

		${Bullet_LIBRARIES}
	)

	#Experimental
	add_executable(opengl_test ${EXPERIMENTAL_OPENGL})
	add_dependencies(opengl_test projectdraw_core projectdraw_physics3d)
	target_link_libraries(opengl_test
		${FREETYPE_LIBRARIES} 
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARIES}
		${Assimp_LIBRARIES}
		${SFML_LIBRARIES}
		debug /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libprojectdraw_core.a
		optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libprojectdraw_core.a
		debug /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libprojectdraw_physics3d.a
		optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libprojectdraw_physics3d.a
		
		${Bullet_LIBRARIES}
	)

endif()