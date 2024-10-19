
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")


set(CORE_SRC
	Util/Transform/rotator.cpp
	Util/Transform/moveable.h
	Util/Transform/rotator.h
	Util/Transform/moveable.cpp
	Util/Debug/debug.hpp
	Util/Debug/logging.cpp
	Util/Debug/logging.h
	Util/Debug/vector.h
	Util/Debug/vector.cpp
	Util/Mouse/mouse.cpp
	Util/Mouse/mouse.h
	Util/Config/configurator.h
	Util/Config/configurator.cpp
	core.cpp
	Resource/resourcemanager.h
	Resource/resourcemanager.cpp
	Texture/texture.h
	Texture/Filter/SSAO/ssao.cpp
	Texture/Filter/SSAO/ssao.h
	Texture/Filter/filter.cpp
	Texture/Filter/Antialias/smaa.cpp
	Texture/Filter/Antialias/smaa.h
	Texture/Filter/filter.h
	Texture/Filter/HDR/hdr.cpp
	Texture/Filter/HDR/hdr.h
	Texture/Filter/Bloom/bloom.cpp
	Texture/Filter/Bloom/bloom.h
	Texture/texture.cpp
	Scene/drawable.cpp
	Scene/Light/light.h
	Scene/Light/lights.h
	Scene/Light/slight.h
	Scene/Light/slight.cpp
	Scene/Light/dlight.cpp
	Scene/Light/light.cpp
	Scene/Light/dlight.h
	Scene/Light/plight.h
	Scene/Light/plight.cpp
	Scene/sceneobject.h
	Scene/sceneobject.cpp
	Scene/UI/HUD/hud.h
	Scene/UI/HUD/hud.cpp
	Scene/UI/Components/Button/button.cpp
	Scene/UI/Components/Button/button.h
	Scene/UI/Components/uicomponent.h
	Scene/UI/Components/Text/textbox.cpp
	Scene/UI/Components/Text/textblock.cpp
	Scene/UI/Components/Text/textblock.h
	Scene/UI/Components/Text/textbox.h
	Scene/UI/Components/uicomponent.cpp
	Scene/Animation/animatable.h
	Scene/Scenes/scene2d.h
	Scene/Scenes/scene.cpp
	Scene/Scenes/subscene.cpp
	Scene/Scenes/imgscene.cpp
	Scene/Scenes/scene.h
	Scene/Scenes/imgscene.h
	Scene/Scenes/subscene.h
	Scene/Particle/2D/billboard_generator.h
	Scene/Particle/2D/point_generator.h
	Scene/Particle/2D/billboard_generator.cpp
	Scene/Particle/2D/point_generator.cpp
	Scene/Particle/particlegenerator.cpp
	Scene/Particle/particlegenerator.h
	Scene/Particle/3D/billboard_generator.h
	Scene/Particle/3D/point_generator.h
	Scene/Particle/3D/billboard_generator.cpp
	Scene/Particle/3D/point_generator.cpp
	Scene/Particle/particles.h
	Scene/Text/font.cpp
	Scene/Text/text.cpp
	Scene/Text/text.h
	Scene/Text/multiline_text.cpp
	Scene/Text/font.h
	Scene/Text/multiline_text.h
	Scene/ascene.cpp
	Scene/Camera/Util/freecamera.h
	Scene/Camera/Util/freecamera.cpp
	Scene/Camera/camera.h
	Scene/Camera/camera.cpp
	Scene/Camera/orthocamera.h
	Scene/Camera/orthocamera.cpp
	Scene/Camera/perspcamera.h
	Scene/Camera/perspcamera.cpp
	Scene/ascene.h
	Scene/drawable.h
	common.h
	core.h
	Renderer/Interface/renderobject.cpp
	Renderer/Interface/renderobject.h
	Renderer/arenderer.cpp
	Renderer/ForwardRenderer/Interface/forward_sceneobject.h
	Renderer/ForwardRenderer/Interface/forward_sceneobject.cpp
	Renderer/ForwardRenderer/Interface/forward_text.h
	Renderer/ForwardRenderer/Interface/Particle/2D/fbillboard_generator.cpp
	Renderer/ForwardRenderer/Interface/Particle/2D/fpoint_generator.h
	Renderer/ForwardRenderer/Interface/Particle/2D/fbillboard_generator.h
	Renderer/ForwardRenderer/Interface/Particle/2D/fpoint_generator.cpp
	Renderer/ForwardRenderer/Interface/Particle/3D/fbillboard_generator.cpp
	Renderer/ForwardRenderer/Interface/Particle/3D/fpoint_generator.h
	Renderer/ForwardRenderer/Interface/Particle/3D/fbillboard_generator.h
	Renderer/ForwardRenderer/Interface/Particle/3D/fpoint_generator.cpp
	Renderer/ForwardRenderer/Interface/forward_drawable.h
	Renderer/ForwardRenderer/Interface/forward_drawable.cpp
	Renderer/ForwardRenderer/Interface/forward_mesh.cpp
	Renderer/ForwardRenderer/Interface/forward_text.cpp
	Renderer/ForwardRenderer/Interface/forward_mesh.h
	Renderer/ForwardRenderer/Basic/forward.cpp
	Renderer/ForwardRenderer/Basic/forward.h
	Renderer/arenderer.h
	Renderer/FlatRenderer/flat.h
	Renderer/FlatRenderer/Interface/flat_sceneobject.cpp
	Renderer/FlatRenderer/Interface/flat_sceneobject.h
	Renderer/FlatRenderer/Interface/flat_text.h
	Renderer/FlatRenderer/Interface/flat_drawable.cpp
	Renderer/FlatRenderer/Interface/flat_multilinetext.cpp
	Renderer/FlatRenderer/Interface/flat_mesh.cpp
	Renderer/FlatRenderer/Interface/Particle/flat_billboard_generator.h
	Renderer/FlatRenderer/Interface/Particle/flat_billboard_generator.cpp
	Renderer/FlatRenderer/Interface/Particle/flat_point_generator.h
	Renderer/FlatRenderer/Interface/Particle/flat_point_generator.cpp
	Renderer/FlatRenderer/Interface/flat_mesh.h
	Renderer/FlatRenderer/Interface/flat_text.cpp
	Renderer/FlatRenderer/Interface/flat_drawable.h
	Renderer/FlatRenderer/Interface/flat_multilinetext.h
	Renderer/FlatRenderer/flat.cpp
	Renderer/DeferredRenderer/drawable_util.cpp
	Renderer/DeferredRenderer/Interface/deferred_mesh.cpp
	Renderer/DeferredRenderer/Interface/Light/deferred_dlight.h
	Renderer/DeferredRenderer/Interface/Light/deferred_plight.h
	Renderer/DeferredRenderer/Interface/Light/deferred_plight.cpp
	Renderer/DeferredRenderer/Interface/Light/deferred_slight.h
	Renderer/DeferredRenderer/Interface/Light/deferred_slight.cpp
	Renderer/DeferredRenderer/Interface/Light/deferred_dlight.cpp
	Renderer/DeferredRenderer/Interface/deferred_drawable.cpp
	Renderer/DeferredRenderer/Interface/deferred_sceneobject.cpp
	Renderer/DeferredRenderer/Interface/deferred_sceneobject.h
	Renderer/DeferredRenderer/Interface/deferred_drawable.h
	Renderer/DeferredRenderer/Interface/deferred_text.h
	Renderer/DeferredRenderer/Interface/deferred_mesh.h
	Renderer/DeferredRenderer/Interface/deferred_text.cpp
	Renderer/DeferredRenderer/light_util.cpp
	Renderer/DeferredRenderer/deferred.cpp
	Renderer/DeferredRenderer/drawable_util.h
	Renderer/DeferredRenderer/deferred.h
	Renderer/DeferredRenderer/light_util.h
	Mesh/Primitives/pgenerator.cpp
	Mesh/Primitives/Image/image.h
	Mesh/Primitives/mesh.cpp
	Mesh/Primitives/primitives.h
	Mesh/Primitives/mesh.h
	Mesh/Primitives/pgenerator.h
	Mesh/Util/box.h
	Mesh/Util/box.cpp
	Shader/shader.h
	Shader/shader.cpp
)
find_package(Stb REQUIRED)

find_package(glm CONFIG REQUIRED)

find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)


add_library(core ${CORE_SRC}) 
target_include_directories(core PRIVATE	${Stb_INCLUDE_DIR} PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(core  
	PRIVATE
	GLEW::GLEW
	OpenGL::GL
	Freetype::Freetype
	assimp::assimp
	PUBLIC
	glm::glm-header-only
)

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