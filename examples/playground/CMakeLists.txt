find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_executable(playground 
    src/main.cpp
)
#target_compile_definitions(playground PRIVATE ASSETS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
target_link_libraries(playground PRIVATE physics window sfml-window)