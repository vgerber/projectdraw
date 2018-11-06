cmake_minimum_required (VERSION 2.8)
project (Demo_3d)

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

add_executable(3d_demo ${SRC_FOLDER})

target_link_libraries(3d_demo
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

if(WIN32)

include_directories(C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/include)
include_directories(C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/src)


#add_executable(engine src/main.cpp ${ENGINE_SRC})
add_executable(texture_demo ${SRC_FOLDER})
target_link_libraries(texture_demo
#ASSIMP
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/assimp-vc140-mtd.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/assimp-vc140-mt.lib
#GLEW
	${GLEW_LIBRARIES}
	debug 	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/glew32d.lib 
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/glew32.lib 
#SOIL
	#debug 	  C:/Users/Vincent/Documents/Projects/Lib/C++/SOIL/projects/VC9/Debug/SOIL.lib
	#optimized C:/Users/Vincent/Documents/Projects/Lib/C++/SOIL/projects/VC9/Release/SOIL.lib	
#OpenGL
	debug	  C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/OpenGL32.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/OpenGL32.lib
#FreeType
	${Freetype_LIBRARIES}
	#debug 	  C:/Users/Vincent/Documents/Projects/Lib/C++/freetype-2.9.1/objs/Win32/Debug/freetype.lib 
	#optimized C:/Users/Vincent/Documents/Projects/Lib/C++/freetype-2.9.1/objs/Win32/Release/freetype.lib 
#SFML
	debug     C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/sfml-graphics-d.lib
	debug     C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/sfml-window-d.lib
	debug     C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/debug/lib/sfml-system-d.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/sfml-graphics.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/sfml-window.lib
	optimized C:/Users/Vincent/Documents/Projects/Lib/vcpkg/installed/x64-windows/lib/sfml-system.lib
	#${SFML_LIBRARIES}
#ProjectDraw
	debug 	   C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Debug/projectdraw_slim.lib
	optimized  C:/Users/Vincent/Documents/Projects/C++/projectdraw_slim/build/Release/projectdraw_slim.lib
)

endif()
