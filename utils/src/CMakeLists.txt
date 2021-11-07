cmake_minimum_required(VERSION 3.11)

# SOURCES
set(SOURCES
    "GL_init.cpp"
    "GLSL_loader.cpp")

# OUTPUT
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR})
add_library(${LIB_NAME} STATIC ${SOURCES})
target_link_libraries(${LIB_NAME}
                      glfw
                      OpenGL::GL
                      ${GLEW_LIBRARIES}
                      ${GLM_LIBRARIES})
