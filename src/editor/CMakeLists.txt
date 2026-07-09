set(EDITOR_SOURCES
    editorLayer.cpp
    editorLayer.h
)

add_library(editor
    STATIC ${EDITOR_SOURCES}
)

target_link_libraries(editor
    PRIVATE core
    PRIVATE glfw
)

target_include_directories(editor
    PRIVATE "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/renderer/glad/include"
    "${CMAKE_SOURCE_DIR}/ext/glfw/include"
)

