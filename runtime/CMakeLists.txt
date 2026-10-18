
add_executable(gameRuntime)

target_sources(gameRuntime
    PRIVATE
    Runtime.cpp
)

target_link_libraries(gameRuntime
    PRIVATE
    application
)

enable_engine_warnings(gameRuntime)
