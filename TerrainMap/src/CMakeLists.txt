cmake_minimum_required(VERSION 3.11)

# PACKAGES
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR}
                    ${GLEW_INCLUDE_DIRS}
                    ${GLM_INCLUDE_DIRS})

# SOURCES
set(SOURCES
    GLSLloader.cpp
    Area.cpp
    Earth.cpp
    Detailing.cpp
    Camera.cpp
    TerrainMap.cpp)

# OUTPUT
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXE_OUTPUT_DIR}")
add_executable(TerrainMap ${SOURCES})
target_link_libraries(TerrainMap
                      glfw
                      OpenGL::GL
                      ${GLEW_LIBRARIES}
                      ${GLM_LIBRARIES})
