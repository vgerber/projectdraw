cmake_minimum_required (VERSION 2.8)
project (Demo_Texture)

file(GLOB_RECURSE SRC_FOLDER
	"src/*.h"
	"src/*.cpp"
)

if (UNIX)

find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)


include_directories(
	/home/vincent/Development/Lib/Cpp/glm-0.9.9.0/
	/home/vincent/Development/Cpp/projectdraw_slim/src/
	/home/vincent/Development/Lib/Cpp/stb-master/
	${FREETYPE_INCLUDE_DIRS} 
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
)

add_executable(texture_demo ${SRC_FOLDER})

target_link_libraries(texture_demo 
	sfml-graphics
	sfml-window
	sfml-system
	/home/vincent/Development/Cpp/projectdraw_slim/build/libprojectdraw_slim.a
	${FREETYPE_LIBRARIES} 
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES} 

	debug 	  /home/vincent/Development/Lib/Cpp/assimp-master/build/code/libassimp.so 
	optimized /home/vincent/Development/Lib/Cpp/assimp-master/Release/code/libassimp.so 
)

endif()
