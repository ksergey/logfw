cmake_minimum_required(VERSION 3.0)

# project name
project(logfw)

# setting the cpp standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# options
option(logfw_BUILD_EXAMPLES "Build library examples" ON)

# in case of build in scope of parent project
get_directory_property(has_parent PARENT_DIRECTORY)
if (has_parent)
    set(logfw_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
endif()

if (logfw_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
