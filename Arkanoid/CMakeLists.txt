cmake_minimum_required(VERSION 3.11)
project(Arkanoid VERSION 2.1)

# PROPERTIES
set(EXE_NAME Arkanoid)

set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(OUTPUT_DIR "${PROJECT_SOURCE_DIR}/buildOut")
set(EXE_OUTPUT_DIR "${OUTPUT_DIR}/bin")

# COMPILER
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unknown-pragmas -O2")

# SOURCES
include_directories(${INCLUDE_DIR})

# OUTPUT
add_subdirectory(${SRC_DIR})
