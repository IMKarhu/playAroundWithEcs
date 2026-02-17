add_library(windowing
    window.h
    window.cpp
)

target_include_directories(windowing
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
