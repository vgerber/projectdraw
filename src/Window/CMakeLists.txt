find_package(SFML COMPONENTS window CONFIG REQUIRED)

set(WINDOW_SRC window.h window.cpp Mouse/mouse.h Mouse/mouse.cpp)
add_library(window ${WINDOW_SRC}) 
target_include_directories(window PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(window PUBLIC core sfml-window)