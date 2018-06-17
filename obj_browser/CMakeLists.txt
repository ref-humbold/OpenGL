cmake_minimum_required(VERSION 3.5)

# PROJECT
project(ObjBrowser VERSION 1.0)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

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
    "${PROJECT_SOURCE_DIR}/src/FileReader.cpp"
    "${PROJECT_SOURCE_DIR}/src/Camera.cpp"
    "${PROJECT_SOURCE_DIR}/src/GraphicObject.cpp"
    "${PROJECT_SOURCE_DIR}/src/ObjBrowser.cpp")

# COMPILER
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# OUTPUT
add_executable(ObjBrowser ${SOURCES})
target_link_libraries(ObjBrowser
                      glfw
                      ${OPENGL_LIBRARIES}
                      ${GLEW_LIBRARIES})
