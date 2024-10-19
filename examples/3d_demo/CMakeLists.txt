find_package(SFML COMPONENTS window CONFIG REQUIRED)

add_executable(3d_demo src/main.cpp) 
target_link_libraries(3d_demo PRIVATE core sfml-window)