add_library(ecsImpl
    components.h
    ecs.h
    ecs.cpp
    componentpool.h
    view.h
)

target_include_directories(ecsImpl
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)


