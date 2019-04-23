cmake_minimum_required(VERSION 3.11)
project(TerrainMap VERSION 1.0)

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
include_directories("${PROJECT_SOURCE_DIR}/include")
set(SOURCES "${PROJECT_SOURCE_DIR}/src/GLSLloader.cpp"
            "${PROJECT_SOURCE_DIR}/src/Area.cpp"
            "${PROJECT_SOURCE_DIR}/src/Earth.cpp"
            "${PROJECT_SOURCE_DIR}/src/Detailing.cpp"
            "${PROJECT_SOURCE_DIR}/src/Camera.cpp"
            "${PROJECT_SOURCE_DIR}/src/TerrainMap.cpp")

# OUTPUT
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
add_executable(TerrainMap ${SOURCES})
target_link_libraries(TerrainMap glfw
                                 OpenGL::GL
                                 ${GLEW_LIBRARIES}
                                 ${GLM_LIBRARIES})
