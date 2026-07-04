add_executable(game main.cpp application.h application.cpp gameLayer.h gameLayer.cpp basePass.h basePass.cpp screenPass.h screenPass.cpp)

set_property(TARGET game PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/buildvs")

target_include_directories(game
    PRIVATE "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(game
    PRIVATE core renderer editor
)

add_custom_command(
    TARGET game
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    "${CMAKE_BINARY_DIR}/assets"
)

add_custom_command(
    TARGET game
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/src/game/shaders"
    "${CMAKE_BINARY_DIR}/shaders"
)
