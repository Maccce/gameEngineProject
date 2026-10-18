add_library(application)

target_sources(application
    PRIVATE
    Application.cpp
    Project.cpp
    ProjectLoader.cpp
    ProjectRegistry.cpp
    SharedLibraryLinux.cpp
)

target_include_directories(application
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(application
    PUBLIC
    engine
    PRIVATE
    nlohmann_json::nlohmann_json
    ${CMAKE_DL_LIBS}
)
