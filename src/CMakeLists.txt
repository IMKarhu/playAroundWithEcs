add_executable(ecs
    main.cpp
)

# add_subdirectory(ext)
add_subdirectory(windowing)
add_subdirectory(ecsImpl)
add_subdirectory(renderer)
add_subdirectory(systems)

target_link_libraries(ecs
    PRIVATE windowing
    PRIVATE ecsImpl
    PRIVATE renderer
    PRIVATE systems
    PUBLIC glfw
    PUBLIC glad
    PUBLIC imgui
    PRIVATE OpenGL::GL
    PRIVATE glm::glm

)

target_link_libraries(windowing PUBLIC glfw glad)
target_link_libraries(systems PUBLIC ecsImpl glm::glm windowing renderer)
target_link_libraries(renderer PUBLIC ecsImpl glm::glm glad imgui windowing)
