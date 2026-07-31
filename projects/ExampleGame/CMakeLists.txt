add_library(exampleGame SHARED)

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
