find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_executable(texture_demo src/main.cpp)
target_link_libraries(texture_demo PRIVATE core sfml-window)