file(GLOB_RECURSE RENDERER_SOURCES
    "opengl/*.cpp"
    "opengl/glad/src/glad.c"
    "vulkan/*.cpp"
    "opengl/*.h"
    "opengl/*.hpp"
    "vulkan/*.h"
    "renderer.h"
    "framebuffer.cpp"
    "framebufferBuilder.cpp"
    "framebufferManager.cpp"
    "framebuffer.h"
    "framebufferBuilder.h"
    "framebufferManager.h"
    "renderInfo.h"
    "renderPassDesc.h"
    "shader.h"
)

add_library(renderer
    SHARED ${RENDERER_SOURCES}
)

target_link_libraries(renderer
    PRIVATE core
)

target_include_directories(renderer
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE "opengl/glad/include"
)

target_compile_definitions(renderer
    PRIVATE RENDERER_EXPORT
)
