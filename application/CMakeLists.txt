add_library(application)

target_sources(application
    PRIVATE
    Application.cpp
    Project.cpp
    ProjectLoader.cpp
    ProjectRegistry.cpp
)

target_include_directories(application
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(application
    PRIVATE
    engine
    nlohmann_json::nlohmann_json
)
