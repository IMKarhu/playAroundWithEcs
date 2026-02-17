add_library(systems
    renderSystem.h
    renderSystem.cpp
)

target_include_directories(systems
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
