# Copyright (C) 2024 Gilles Degottex - All Rights Reserved
#
# You may use, distribute and modify this code under the
# terms of the Apache 2.0 license. You should have
# received a copy of this license with this file.
# If not, please visit:
#     https://github.com/gillesdegottex/cpptopdb

cmake_minimum_required(VERSION 3.22)

project(cpptopdb)

# Compilation options ---------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # To generate compile_commands.json to allow clangd do the code navigation
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

option(CPPTOPDB_TESTS "Build the tests" OFF)

# Default to Release
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

if (WIN32)
  # TODO 
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-parameter")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-parameter")
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()
