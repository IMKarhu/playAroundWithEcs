add_executable(game main.cpp application.h application.cpp gameLayer.h gameLayer.cpp)

target_include_directories(game
    PRIVATE "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(game
    PRIVATE core renderer editor
)
