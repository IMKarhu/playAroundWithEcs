add_library(renderer
    framebuffer.h
    renderer.h
    glframebuffer.h
    glframebuffer.cpp
    glrenderer.h
    glrenderer.cpp
    shader.h
    shader.cpp
    imguiLayer.h
    imguiLayer.cpp
)

target_include_directories(renderer
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
