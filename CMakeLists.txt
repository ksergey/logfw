cmake_minimum_required(VERSION 3.0)

# project name
project(logfw)

# setting the cpp standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# options
option(BUILD_EXAMPLES "Build library examples" ON)

# create library entry
add_library(logfw INTERFACE)
target_include_directories(logfw INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (BUILD_EXAMPLES)
    message(STATUS "logfw: build examples (yes)")
    add_subdirectory(examples)
else()
    message(STATUS "logfw: build examples (no)")
endif()
