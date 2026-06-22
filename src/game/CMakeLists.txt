add_executable(game main.cpp application.h application.cpp gameLayer.h gameLayer.cpp)

set_property(TARGET game PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/buildvs")

target_include_directories(game
    PRIVATE "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(game
    PRIVATE core renderer editor
)
