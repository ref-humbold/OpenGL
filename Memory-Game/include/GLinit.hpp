#ifndef GL_INIT_HPP_
#define GL_INIT_HPP_

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <tuple>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLSLloader.hpp"

struct gl_exception : std::runtime_error
{
    explicit gl_exception(const std::string & s) : std::runtime_error(s)
    {
    }
};

void createVertexArray();

void glfwHints();

std::tuple<GLFWwindow *, GLuint> initializeGL();

#endif
