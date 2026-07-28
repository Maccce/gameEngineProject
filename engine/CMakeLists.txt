add_library(engine STATIC)


target_sources(engine
    PRIVATE

    core/Engine.cpp
    core/Application.cpp
    core/Time.cpp
)


target_include_directories(engine

    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}

)


target_compile_features(engine
    PUBLIC
    cxx_std_20
)


enable_engine_warnings(engine)
