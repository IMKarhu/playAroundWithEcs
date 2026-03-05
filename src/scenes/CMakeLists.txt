add_library(scenes
    scene.h
    scene.cpp
)

target_include_directories(scenes
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
