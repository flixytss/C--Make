cmake_minimum_required(VERSION 3.19)

project(c++make
    LANGUAGES CXX
    VERSION 2.0
    DESCRIPTION "A Makefile Generator, Just that..."
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(UNITY_BUILD_MODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_COMPILER_LAUNCHER ccache)

set(SOURCES
    src/main.cc
    src/files.cc
    src/make.cc
    src/inputfile.cc
    src/sync.cc
    src/install.cc
)

add_executable(c++make ${SOURCES})

target_include_directories(c++make
    PUBLIC
        includes
)
option(ARGUMENTSEA "Enable argumentsea" OFF)

if(ARGUMENTSEA)
    message(STATUS "Argumentsea enabled...")
    target_compile_definitions(c++make PRIVATE ARGUMENTSEA)
endif()