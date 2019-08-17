cmake_minimum_required(VERSION 3.11)
project(Memory-Game VERSION 1.0)

# COMPILER
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unknown-pragmas")

# PACKAGES
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR}
                    ${GLEW_INCLUDE_DIRS}
                    ${GLM_INCLUDE_DIRS})

# SOURCES
include_directories(include)
set(SOURCES src/GLSLloader.cpp
            src/GameController.cpp
            src/MemoryGame.cpp)

# OUTPUT
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
add_executable(MemoryGame ${SOURCES})
target_link_libraries(MemoryGame glfw
                                 OpenGL::GL
                                 ${GLEW_LIBRARIES}
                                 ${GLM_LIBRARIES})
