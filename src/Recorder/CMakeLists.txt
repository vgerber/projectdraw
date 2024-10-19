find_package(FFMPEG REQUIRED)

set(RECORDER_SRC
    glrecorder.cpp
    glrecorder.h
    recorder.cpp
    recorder.h
)

add_library(recorder ${RECORDER_SRC}) 
target_include_directories(recorder PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_directories(recorder PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(recorder PRIVATE ${FFMPEG_LIBRARIES} PUBLIC core)