cmake_minimum_required(VERSION 3.20)
project(MyWasmProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

# =========================
# Core library
# =========================
add_library(engine_core
    src/core/engine.cpp
)

target_include_directories(engine_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(engine_core PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(engine_core PRIVATE /W4)
else()
    target_compile_options(engine_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

# =========================
# Native tests only
# =========================
if(BUILD_TESTING AND NOT EMSCRIPTEN)
    include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.1
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(engine_tests
        tests/engine_tests.cpp
    )

    target_link_libraries(engine_tests
        PRIVATE
            engine_core
            Catch2::Catch2WithMain
    )

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(Catch)
    catch_discover_tests(engine_tests)
endif()

# =========================
# WASM target
# =========================
if(EMSCRIPTEN)
    add_executable(engine_wasm
        src/wasm/bindings.cpp
    )

    target_link_libraries(engine_wasm
        PRIVATE
            engine_core
    )

    target_compile_options(engine_wasm PRIVATE
        -O2
    )

    target_link_options(engine_wasm PRIVATE
        -O2
        --bind
        -sMODULARIZE=1
        -sEXPORT_ES6=1
        -sENVIRONMENT=web
        -sALLOW_MEMORY_GROWTH=1
        -sFILESYSTEM=0
        -sASSERTIONS=1
    )

    set_target_properties(engine_wasm PROPERTIES
        OUTPUT_NAME "engine"
        SUFFIX ".js"
    )
endif()