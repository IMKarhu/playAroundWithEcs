add_library(ecsImpl INTERFACE
    components.h
    ecs.h
    ecs.cpp
    componentpool.h
    view.h
)

target_include_directories(ecsImpl
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)


