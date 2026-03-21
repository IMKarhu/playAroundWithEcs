add_executable(ecs
    main.cpp
)

# add_subdirectory(ext)
add_subdirectory(windowing)
add_subdirectory(ecsImpl)
add_subdirectory(renderer)
add_subdirectory(systems)
add_subdirectory(scenes)

target_link_libraries(ecs
    PRIVATE windowing
    PRIVATE ecsImpl
    PRIVATE renderer
    PRIVATE systems
    PRIVATE scenes
    PUBLIC glfw
    PUBLIC glad
    PUBLIC imgui
    PRIVATE OpenGL::GL
    PRIVATE glm::glm

)

target_link_libraries(windowing PUBLIC glfw glad)
target_link_libraries(systems PUBLIC ecsImpl glm::glm windowing renderer scenes)
target_link_libraries(renderer PUBLIC ecsImpl glm::glm glad imgui windowing)
target_link_libraries(scenes PUBLIC ecsImpl glm::glm)
