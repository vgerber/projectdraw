find_package(SFML COMPONENTS window CONFIG REQUIRED)

add_executable(recorder_demo src/main.cpp) 
target_link_libraries(recorder_demo PRIVATE core recorder sfml-window)