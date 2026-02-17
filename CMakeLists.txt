cmake_minimum_required(VERSION 3.28...4.1)

project(TestingGround)

#set(CMAKE_CXX_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/bin/clang++")
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "/EHsc")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
add_subdirectory(ext)
add_subdirectory(src)

# add_executable(Ecs
#     main.cpp
#     ecs.h
#     window.h
#     window.cpp
#     renderSystem.h
#     renderSystem.cpp
#     imguiLayer.h
#     imguiLayer.cpp
#     framebuffer.h
#     glframebuffer.h
#     glframebuffer.cpp
#     renderer.h
#     glrenderer.h
#     glrenderer.cpp
#     components.h
#     shader.h
#     shader.cpp
# )

# target_link_libraries(Ecs
#     PUBLIC glfw
#     PUBLIC glad
#     PUBLIC imgui
#     PRIVATE OpenGL::GL
#     PRIVATE glm::glm
# )

# for copying compile_commands to root
set(SRC_COMPILE_COMMANDS "${CMAKE_BINARY_DIR}/compile_commands.json")
set(DST_COMPILE_COMMANDS "${CMAKE_SOURCE_DIR}/compile_commands.json")
add_custom_command(
    OUTPUT ${DST_COMPILE_COMMANDS}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SRC_COMPILE_COMMANDS}
            ${DST_COMPILE_COMMANDS}
    DEPENDS ${SRC_COMPILE_COMMANDS}
    COMMENT "Copying compile_commands.json to root"
)

add_custom_target(copy_compile_commands ALL
    DEPENDS ${DST_COMPILE_COMMANDS}
)
