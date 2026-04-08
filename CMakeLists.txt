cmake_minimum_required(VERSION 3.16)
project(slae_benchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_SOURCES
    linear_algebra.cpp
    solvers.cpp
    generators.cpp
    experiments.cpp
    report_io.cpp
)

add_executable(slae_benchmark main.cpp ${COMMON_SOURCES})
add_executable(slae_tests tests.cpp ${COMMON_SOURCES})

foreach(target slae_benchmark slae_tests)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()
