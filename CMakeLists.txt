cmake_minimum_required(VERSION 3.0)

# project name
project(logfw)

# Default build type
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
endif()

# options
option(LogFW_BUILD_EXAMPLES "Build library examples" ON)

# create library entry
add_library(logfw INTERFACE)
add_library(ksergey::logfw ALIAS logfw)
target_include_directories(logfw INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(logfw INTERFACE cxx_std_14)

if (LogFW_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
