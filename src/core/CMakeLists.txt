file(GLOB_RECURSE CORE_SOURCES
    "windowing/*.cpp"
    "ecsImpl/*.h"
    "windowing/*.h"
    "platform.h"
    "layer.h"
    # "modelImporter.h"
    # "modelImporter.cpp"
    "assetbase.h"
    "string_hash.h"
    "assetManager.cpp"
    "assetManager.h"
    "textureManager.h"
    "textureManager.cpp"
    "meshManager.h"
    "meshManager.cpp"
    "tiny_gltf_v3.h"
    "tiny_gltf_v3.c"
    "tinygltf_json.h"
    "tinygltf_json_c.h"
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
