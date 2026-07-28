add_library(engine STATIC)


target_sources(engine
    PRIVATE

    core/Engine.cpp
    core/Application.cpp
    core/Time.cpp
    core/Log.cpp
)


target_include_directories(engine

    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}

)


target_compile_features(engine
    PUBLIC
    cxx_std_20
)


target_link_libraries(engine

    PUBLIC

    spdlog::spdlog

)

enable_engine_warnings(engine)
