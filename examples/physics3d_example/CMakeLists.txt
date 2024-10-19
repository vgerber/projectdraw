find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_executable(physics3d_example 
    src/main.cpp 
    src/vehicle.cpp 
    src/vehicle.h
) 
target_link_libraries(physics3d_example PRIVATE physics sfml-window)