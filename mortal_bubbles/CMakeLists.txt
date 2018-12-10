cmake_minimum_required(VERSION 3.5)
project(MortalBubbles VERSION 1.0)

# COMPILER
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unknown-pragmas")

# PACKAGES
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS}
                    ${OPENGL_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# SOURCES
include_directories("${PROJECT_SOURCE_DIR}/include")
set(SOURCES
    "${PROJECT_SOURCE_DIR}/src/GLSLloader.cpp"
    "${PROJECT_SOURCE_DIR}/src/GameAquarium.cpp"
    "${PROJECT_SOURCE_DIR}/src/GameBubble.cpp"
    "${PROJECT_SOURCE_DIR}/src/GamePlayer.cpp"
    "${PROJECT_SOURCE_DIR}/src/GameController.cpp"
    "${PROJECT_SOURCE_DIR}/src/MortalBubbles.cpp")

# OUTPUT
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
add_executable(MortalBubbles ${SOURCES})
target_link_libraries(MortalBubbles
                      glfw
                      ${OPENGL_LIBRARIES}
                      ${GLEW_LIBRARIES})
