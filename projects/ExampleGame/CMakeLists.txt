add_library(exampleGame SHARED)

set_target_properties(exampleGame PROPERTIES
    OUTPUT_NAME "ExampleGame"
)

target_sources(exampleGame
    PRIVATE
    src/Game.cpp
    src/GameModule.cpp
)

target_include_directories(exampleGame
    PUBLIC
    src
)

target_link_libraries(exampleGame
    PRIVATE
    engine
    application
)
