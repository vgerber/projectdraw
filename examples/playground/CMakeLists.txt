find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_executable(playground src/main.cpp)
target_link_libraries(playground PRIVATE core physics sfml-window)