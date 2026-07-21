file(GLOB_RECURSE CORE_SOURCES
    "windowing/*.cpp"
    "ecsImpl/*.h"
    "windowing/*.h"
    "scene/*.h"
    "scene/*.cpp"
    "scene/*.hpp"
    "assetmanagers/*.h"
    "assetmanagers/*.hpp"
    "assetmanagers/*.cpp"
    "assetmanagers/*.c"
    "platform.h"
    "layer.h"
    "assetbase.h"
    "string_hash.h"
)

add_library(core
    SHARED ${CORE_SOURCES}
)

target_link_libraries(core
    PUBLIC glm
    PUBLIC glfw
)

target_include_directories(core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(core
    PRIVATE CORE_EXPORT
)
