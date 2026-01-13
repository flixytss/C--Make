cmake_minimum_required(VERSION 3.19)

project(C++Make
    LANGUAGES C CXX
    VERSION 1.0
    DESCRIPTION "A Compilation Helper"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/main.cc
    src/files.cc
    src/make.cc
    src/inputfile.cc
)

add_executable(C++Make ${SOURCES})

target_include_directories(C++Make
    PUBLIC
        includes
)