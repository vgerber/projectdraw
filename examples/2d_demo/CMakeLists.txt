find_package(SFML COMPONENTS window CONFIG REQUIRED)

add_executable(2d_demo main.cpp) 
target_link_libraries(2d_demo PRIVATE core sfml-window)