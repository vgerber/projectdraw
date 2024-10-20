find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_executable(opengl_test src/main.cpp)
target_link_libraries(opengl_test PRIVATE core physics sfml-window)