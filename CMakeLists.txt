cmake_minimum_required (VERSION 3.28)
project (3d-playground)

add_subdirectory(src/Core)
add_subdirectory(src/Physics)
add_subdirectory(src/Recorder)
add_subdirectory(examples)

# OpenGL Components
# GL
# GLU   
# GLX   
# EGL   
# OpenGL


# add_library(projectdraw_physics3d ${PHYSICS_SRC})
# add_dependencies(projectdraw_physics3d core)
# include_directories(
# 	${Bullet_INCLUDE_DIRS}
# 	${CMAKE_SOURCE_DIR}/src
# )

# target_link_libraries(projectdraw_physics3d
# 	debug /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libcore.a
# 	optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libcore.a
# )


# if(${SFML_FOUND})
# #Examples
# 	#Scene3D Example
# 	include_directories(
# 		${FREETYPE_INCLUDE_DIRS} 
# 		${stb_INCLUDE_DIRS}
# 		${GLM_INCLUDE_DIRS}
# 		${CMAKE_SOURCE_DIR}/src
# 	)

# 	add_executable(3d_example ${EXAMPLE_3D_SRC})
# 	add_dependencies(3d_example core)
# 	target_link_libraries(3d_example
# 		${FREETYPE_LIBRARIES}  
# 		${SFML_LIBRARIES}
# 		debug     /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libcore.a
# 		optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libcore.a
# 	)
	
# 	add_executable(2d_example ${EXAMPLE_2D_SRC})
# 	add_dependencies(2d_example core)
# 	target_link_libraries(2d_example
# 		${FREETYPE_LIBRARIES}  
# 		${SFML_LIBRARIES}
# 		debug     /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libcore.a
# 		optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libcore.a
# 	)

# 	#Physics3D Example
# 	include_directories(
# 		${FREETYPE_INCLUDE_DIRS} 
# 		${stb_INCLUDE_DIRS}
# 		${GLM_INCLUDE_DIRS}
# 		${CMAKE_SOURCE_DIR}/src
# 	)

# 	add_executable(physics3d_example ${EXAMPLE_PHYSICS3D_SRC})
# 	add_dependencies(physics3d_example core projectdraw_physics3d)
# 	target_link_libraries(physics3d_example
# 		${FREETYPE_LIBRARIES} 
# 		${SFML_LIBRARIES}
# 		debug     /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libcore.a
# 		optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libcore.a
# 		debug     /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libprojectdraw_physics3d.a
# 		optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libprojectdraw_physics3d.a

# 		${BULLET_LIBRARIES}
# 	)

# 	#Experimental
# 	add_executable(opengl_test ${EXPERIMENTAL_OPENGL})
# 	add_dependencies(opengl_test core projectdraw_physics3d)
# 	target_link_libraries(opengl_test
# 		${FREETYPE_LIBRARIES} 
# 		${SFML_LIBRARIES}
# 		debug /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libcore.a
# 		optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libcore.a
# 		debug /home/vincent/Development/Cpp/projectdraw_slim/build/debug/libprojectdraw_physics3d.a
# 		optimized /home/vincent/Development/Cpp/projectdraw_slim/build/release/libprojectdraw_physics3d.a
		
# 		${BULLET_LIBRARIES}
# 	)

# endif()
