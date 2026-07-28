add_executable(gameRuntime)


target_sources(gameRuntime

    PRIVATE

    Runtime.cpp

)


target_link_libraries(gameRuntime

    PRIVATE

    engine

)


enable_engine_warnings(gameRuntime)
